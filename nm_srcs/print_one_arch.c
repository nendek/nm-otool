/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_one_arch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:26:02 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:26:11 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		handle_norm_1(t_env *env, uint32_t i, uint32_t nb)
{
	size_t	magic;

	if (!(move_ptr(env, env->file, env->offset)))
		return (1);
	magic = ((t_symb *)((size_t)env->file + env->offset))->val;
	if (magic == LIB_MAGIC && i == nb)
	{
		handle_lib(env, get_size_lib(env, i));
		return (1);
	}
	return (0);
}

static size_t	handle_norm_2(t_env *env)
{
	size_t		magic;

	if (!(move_ptr(env, env->file, env->offset)))
		return (0);
	magic = ((struct mach_header_64*)((size_t)env->file +
				env->offset))->magic;
	return (magic);
}

static void		handle_norm_3(t_env *env)
{
	handle_64(env, same_32);
	print_list(env, env->lst, OUT_64);
}

void			print_one_arch(t_env *env, uint32_t nb)
{
	struct fat_header		*header;
	struct fat_arch			*arch;
	uint32_t				i;
	size_t					magic;

	i = 0;
	header = (struct fat_header *)env->file;
	arch = (struct fat_arch *)move_ptr(env, env->file, sizeof(*header));
	while (i < indian_32(header->nfat_arch))
	{
		set_env(env, indian_32(arch->offset));
		if ((magic = handle_norm_2(env)) == 0)
			return ;
		if (magic == MH_MAGIC_64 && i == nb)
		{
			handle_norm_3(env);
			return ;
		}
		else if (handle_norm_1(env, i, nb) == 1)
			return ;
		if (!(move_ptr(env, arch, sizeof(*arch))))
			return ;
		arch++;
		i++;
	}
}
