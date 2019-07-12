/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:28:23 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:28:26 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		ft_put_16_bytes(t_env *env, size_t addr, size_t size)
{
	uint8_t		*text;
	size_t		i;

	i = 0;
	text = (uint8_t *)addr;
	while (i < 16 && i < size)
	{
		handle_buff(env, get_char(text[i] >> 4));
		handle_buff(env, get_char(text[i] & 15));
		handle_buff(env, ' ');
		i++;
	}
	handle_buff(env, '\n');
}

void			print_sect_64(t_env *env, uint32_t (*indian)(uint32_t))
{
	size_t		i;
	size_t		size;

	i = 0;
	if (indian == indian_32)
		size = indian_64(env->text_size);
	else
		size = env->text_size;
	ft_putstr(env, "Contents of (__TEXT,__text) section\n", NO_PROT_RD);
	while (i < size)
	{
		ft_putval(env, env->text_addr + i, OUT_64);
		ft_putstr(env, "\t", NO_PROT_RD);
		if (!(move_ptr(env, env->file,
						env->text_offset + i + env->offset + 16)))
			return ;
		ft_put_16_bytes(env, (size_t)env->file +
				(size_t)env->text_offset + i + env->offset, size - i);
		i += 16;
	}
}

static void		get_section_text_64(t_env *env, uint32_t (*indian)(uint32_t))
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
		{
			env->text_offset = indian(sect->offset);
			env->text_size = sect->size;
			if (indian == indian_32)
				env->text_addr = indian_64(sect->addr);
			else
				env->text_addr = same_64(sect->addr);
		}
		if (!(move_ptr(env, sect, sizeof(*sect))))
			return ;
		sect++;
		n_sects--;
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
		if (indian(env->current_lc->cmd) == LC_SEGMENT_64)
			get_section_text_64(env, indian);
		i++;
		if (!(env->current_lc = (struct load_command *)(move_ptr(env,
				env->current_lc, (size_t)indian(env->current_lc->cmdsize)))))
			return ;
	}
}
