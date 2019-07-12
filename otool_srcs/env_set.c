/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:27:48 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:27:49 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	set_env(t_env *env, uint32_t offset)
{
	env->offset = offset;
	env->text_offset = 0;
	env->text_size = 0;
	env->current_lc = NULL;
}

t_env	*init_env(void *file, char *filename, size_t size_file)
{
	t_env	*env;

	if (!(env = malloc(sizeof(t_env))))
		return (NULL);
	env->file = file;
	if (!(env->filename = get_name(env, filename, NO_PROT_RD)))
		return (NULL);
	if (!(env->print_buff = malloc(sizeof(char) * 4096)))
		return (NULL);
	env->text_offset = 0;
	env->text_size = 0;
	env->offset = 0;
	env->current_lc = NULL;
	env->endfile = (void *)((size_t)file + size_file);
	env->index_buff = 0;
	return (env);
}

void	free_env(t_env *env)
{
	free((env->filename));
	free((env->print_buff));
	free(env);
}
