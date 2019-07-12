/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:22:31 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:22:32 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		set_env(t_env *env, uint32_t offset)
{
	env->text_sect = 0;
	env->data_sect = 0;
	env->bss_sect = 0;
	env->index = 1;
	env->offset = offset;
	env->current_lc = NULL;
	env->sym = NULL;
	free_tree(env->lst);
	env->lst = NULL;
}

t_env		*init_env(void *file, char *filename, size_t size_file)
{
	t_env	*env;

	if (!(env = malloc(sizeof(t_env))))
		return (NULL);
	env->file = file;
	if (!(env->filename = get_name(env, filename, NO_PROT_RD)))
		return (NULL);
	if (!(env->print_buff = malloc(sizeof(char) * 4096)))
		return (NULL);
	env->lst = NULL;
	env->text_sect = 0;
	env->data_sect = 0;
	env->bss_sect = 0;
	env->index = 1;
	env->offset = 0;
	env->current_lc = NULL;
	env->sym = NULL;
	env->endfile = (void *)((size_t)file + size_file);
	env->index_buff = 0;
	return (env);
}
