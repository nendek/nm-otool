/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_cigam.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:24:40 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:24:42 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		choose_good_arch(t_env *env,
		uint32_t nb, uint32_t magic, uint32_t i)
{
	if (magic == MH_MAGIC)
	{
		handle_32(env, &same_32);
		print_filename(env, nb, magic, i);
		print_list(env, env->lst, OUT_32);
	}
	if (magic == MH_CIGAM)
	{
		handle_32(env, &indian_32);
		print_filename(env, nb, magic, i);
		print_list(env, env->lst, OUT_32);
	}
}

static void		print_all_arches(t_env *env, uint32_t nb)
{
	struct fat_header		*header;
	struct fat_arch			*arch;
	uint32_t				i;
	uint32_t				magic;

	i = 0;
	header = (struct fat_header *)env->file;
	arch = (struct fat_arch *)move_ptr(env, env->file, sizeof(*header));
	while (i < indian_32(header->nfat_arch))
	{
		set_env(env, indian_32(arch->offset));
		if (!(move_ptr(env, env->file, env->offset)))
			return ;
		magic = ((struct mach_header_64*)((size_t)env->file +
					env->offset))->magic;
		choose_good_arch(env, nb, magic, i);
		if (!(move_ptr(env, arch, sizeof(*arch))))
			return ;
		arch++;
		i++;
	}
}

void			handle_fat_cigam(t_env *env)
{
	uint32_t	i;

	i = find_cpu_arch(env);
	if (i == 0)
		print_one_arch(env, find_cpu_arch_nb(env));
	else
		print_all_arches(env, i);
}
