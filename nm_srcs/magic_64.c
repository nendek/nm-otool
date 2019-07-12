/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:25:37 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:25:40 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		handle_sect_symbol_64(t_env *env, t_symb *tmp,
		char symbol, struct nlist_64 *ptr)
{
	if (symbol == 0x0)
		tmp->symb = 'U';
	else if (symbol == 0xC)
		tmp->symb = 'U';
	else if (symbol == 0x2)
		tmp->symb = 'A';
	else if (symbol == 0xA)
		tmp->symb = 'I';
	else if (symbol == 0xE)
	{
		if (ptr->n_sect == env->text_sect)
			tmp->symb = 'T';
		else if (ptr->n_sect == env->data_sect)
			tmp->symb = 'D';
		else if (ptr->n_sect == env->bss_sect)
			tmp->symb = 'B';
		else
			tmp->symb = 'S';
	}
}

static void		handle_type_64(t_env *env, struct nlist_64 *ptr,
		char *string, uint32_t (*indian)(uint32_t))
{
	char	symbol;
	t_symb	*tmp;

	symbol = ptr->n_type & 0xE;
	if ((ptr->n_type & 0xE0) == 0)
	{
		if (!(tmp = malloc(sizeof(t_symb))))
			return ;
		if (indian == &same_32)
			tmp->val = ptr->n_value;
		else
			tmp->val = indian_64(ptr->n_value);
		handle_sect_symbol_64(env, tmp, symbol, ptr);
		if ((ptr->n_type & 0x1) == 0)
			tmp->symb += 32;
		if (!(tmp->name = get_name(env, string, PROT_RD)))
			return ;
		tmp->left = NULL;
		tmp->right = NULL;
		ft_push(env, &(env->lst), tmp);
	}
}

static void		construct_list_64(t_env *env, uint32_t (*indian)(uint32_t))
{
	uint32_t			i;
	char				*string;
	struct nlist_64		*ptr;

	if (!(env->sym))
		return ;
	i = 0;
	if (!(ptr = (struct nlist_64 *)(move_ptr(env, env->file,
						(size_t)indian(env->sym->symoff) + env->offset))))
		return ;
	if (!(string = (char*)(move_ptr(env, env->file, env->offset
	+ (size_t)(indian(ptr->n_un.n_strx)) + (size_t)indian(env->sym->stroff)))))
		return ;
	while (i < indian(env->sym->nsyms))
	{
		handle_type_64(env, ptr, string, indian);
		if (!(move_ptr(env, ptr, sizeof(*ptr))))
			return ;
		ptr++;
		if (!(string = (char*)(move_ptr(env, env->file, env->offset
	+ (size_t)(indian(ptr->n_un.n_strx)) + (size_t)indian(env->sym->stroff)))))
			return ;
		i++;
	}
}

static void		get_sections_number_64(t_env *env, uint32_t (*indian)(uint32_t))
{
	uint32_t					n_sects;
	struct segment_command_64	*cmd;
	struct section_64			*sect;

	cmd = (struct segment_command_64 *)(void *)(env->current_lc);
	if (!(sect = (struct section_64 *)(move_ptr(env,
						cmd, sizeof(struct segment_command_64)))))
		return ;
	n_sects = indian(cmd->nsects);
	while (n_sects > 0)
	{
		if (ft_strcmp(env, sect->sectname, "__text") == 0)
			env->text_sect = env->index;
		if (ft_strcmp(env, sect->sectname, "__data") == 0)
			env->data_sect = env->index;
		if (ft_strcmp(env, sect->sectname, "__bss") == 0)
			env->bss_sect = env->index;
		if (!(move_ptr(env, sect, sizeof(*sect))))
			return ;
		sect++;
		n_sects--;
		env->index += 1;
	}
}

void			handle_64(t_env *env, uint32_t (*indian)(uint32_t))
{
	uint32_t				i;
	struct mach_header_64	*header;

	if (!(header = (struct mach_header_64 *)(move_ptr(env,
						env->file, env->offset))))
		return ;
	if (!(env->current_lc = (struct load_command *)(move_ptr(env,
						env->file, sizeof(*header) + env->offset))))
		return ;
	i = 0;
	while (i < indian(header->ncmds))
	{
		if (indian(env->current_lc->cmd) == LC_SYMTAB)
			env->sym = (struct symtab_command *)env->current_lc;
		if (indian(env->current_lc->cmd) == LC_SEGMENT_64)
			get_sections_number_64(env, indian);
		i++;
		if (!(env->current_lc = (struct load_command *)(move_ptr(env,
				env->current_lc, (size_t)indian(env->current_lc->cmdsize)))))
			return ;
	}
	construct_list_64(env, indian);
}
