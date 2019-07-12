/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:22:20 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:22:21 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint32_t		find_cpu_arch(t_env *env)
{
	uint32_t				i;
	struct fat_arch			*arch;
	struct fat_header		*header;

	i = 0;
	header = (struct fat_header *)env->file;
	arch = (struct fat_arch *)move_ptr(env, env->file, sizeof(*header));
	while (i < indian_32(header->nfat_arch))
	{
		if (arch->cputype == (int32_t)indian_32(CPU_TYPE_X86_64))
			return (0);
		if (!(move_ptr(env, arch, sizeof(*arch))))
			return (0);
		arch++;
		i++;
	}
	return (i);
}

uint32_t		find_cpu_subtype_arch(t_env *env, uint32_t index)
{
	uint32_t				i;
	struct fat_arch			*arch;
	struct fat_header		*header;

	i = 0;
	header = (struct fat_header *)env->file;
	arch = (struct fat_arch *)move_ptr(env, env->file, sizeof(*header));
	while (i < index)
	{
		if (!(move_ptr(env, arch, sizeof(*arch))))
			return (0);
		arch++;
		i++;
	}
	if (indian_32((uint32_t)arch->cpusubtype) == CPU_SUBTYPE_POWERPC_ALL)
	{
		return (1);
	}
	return (0);
}

uint32_t		find_cpu_arch_nb(t_env *env)
{
	uint32_t				i;
	struct fat_arch			*arch;
	struct fat_header		*header;

	i = 0;
	header = (struct fat_header *)env->file;
	arch = (struct fat_arch *)move_ptr(env, env->file, sizeof(*header));
	while (i < indian_32(header->nfat_arch))
	{
		if (arch->cputype == (int32_t)indian_32(CPU_TYPE_X86_64))
			return (i);
		if (!(move_ptr(env, arch, sizeof(*arch))))
			return (0);
		arch++;
		i++;
	}
	return (i);
}

uint32_t		get_size_lib(t_env *env, uint32_t nb)
{
	uint32_t				i;
	struct fat_arch			*arch;
	struct fat_header		*header;

	i = 0;
	header = (struct fat_header *)env->file;
	arch = (struct fat_arch *)move_ptr(env, env->file, sizeof(*header));
	while (i < nb)
	{
		if (!(move_ptr(env, arch, sizeof(*arch))))
			return (0);
		arch++;
		i++;
	}
	return (indian_32(arch->size));
}

char			check_magic(t_env *env)
{
	size_t	magic;
	int		i;

	i = 0;
	if (!(move_ptr(env, env->file, env->offset)))
		return (0);
	magic = ((t_symb *)((size_t)env->file + env->offset))->val;
	if (magic != LIB_MAGIC)
		return (0);
	return (1);
}
