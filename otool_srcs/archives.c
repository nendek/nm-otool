/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archives.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:27:11 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:27:13 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static uint32_t		parse_header_arch(t_env *env, char print)
{
	size_t		name_len;

	name_len = get_name_len(env, print);
	env->offset += 60 + name_len;
	if (!(move_ptr(env, env->file, env->offset)))
		return (0);
	return (*((uint32_t *)((size_t)env->file + env->offset)));
}

static void			get_symbol(t_env *env, uint32_t addr,
		uint32_t end, uint32_t lib_offset)
{
	uint32_t	magic;

	addr += lib_offset;
	end += lib_offset;
	while (addr <= end)
	{
		set_env(env, addr);
		if (!(move_ptr(env, env->file, env->offset + 48)))
			return ;
		addr += ft_atoi(env, (char *)((size_t)env->file +
					env->offset + 48)) + 60;
		magic = parse_header_arch(env, PRINT);
		if (magic == MH_MAGIC_64)
		{
			handle_64(env, same_32);
			print_sect_64(env, same_32);
		}
		if (magic == MH_MAGIC)
		{
			handle_32(env, same_32);
			print_sect_32(env, same_32);
		}
	}
}

static void			get_symbol_in_fat(t_env *env, uint32_t size_lib)
{
	uint32_t	magic;
	uint32_t	i;

	i = env->offset + 8;
	while (i < size_lib)
	{
		set_env(env, i);
		if (!(move_ptr(env, env->file, env->offset)))
			return ;
		i += ft_atoi(env, (char *)((size_t)env->file +
					env->offset + 48)) + 60;
		magic = parse_header_arch(env, PRINT);
		if (magic == MH_MAGIC_64)
		{
			handle_64(env, same_32);
			print_sect_64(env, same_32);
		}
	}
}

static int			update_addr(t_env *env, size_t *i,
		uint32_t *addr, uint32_t *addr2)
{
	if (!(move_ptr(env, env->file, env->offset + *i)))
		return (1);
	if (*(uint32_t *)((size_t)env->file + env->offset + *i) < *addr)
		*addr = *(uint32_t *)((size_t)env->file + env->offset + *i);
	if (*(uint32_t *)((size_t)env->file + env->offset + *i) > *addr2)
		*addr2 = *(uint32_t *)((size_t)env->file + env->offset + *i);
	*i += 8;
	return (0);
}

void				handle_lib(t_env *env, uint32_t size_lib)
{
	uint32_t			size;
	uint32_t			addr;
	uint32_t			addr2;
	size_t				i;
	uint32_t			lib_offset;

	if (check_magic(env) == 0)
		return ;
	lib_offset = env->offset;
	env->offset += 8;
	size = parse_header_arch(env, NO_PRINT);
	i = 8;
	if (!(move_ptr(env, env->file, env->offset + i)))
		return ;
	addr = *(uint32_t *)((size_t)env->file + env->offset + i);
	addr2 = *(uint32_t *)((size_t)env->file + env->offset + i);
	while (i <= size)
		if (update_addr(env, &i, &addr, &addr2) == 1)
			return ;
	if (size == 0)
		get_symbol_in_fat(env, size_lib);
	else
		get_symbol(env, addr, addr2, lib_offset);
}
