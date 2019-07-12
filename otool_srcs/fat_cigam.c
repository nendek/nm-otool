/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_cigam.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:27:56 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:27:57 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		ft_put_16_bytes_ppc(t_env *env, size_t addr, size_t size)
{
	uint8_t		*text;
	size_t		i;

	i = 0;
	text = (uint8_t *)addr;
	while (i < 16 && i < size)
	{
		handle_buff(env, get_char(text[i] >> 4));
		handle_buff(env, get_char(text[i] & 15));
		i++;
		if (i % 4 == 0)
			handle_buff(env, ' ');
	}
	handle_buff(env, '\n');
}

void			print_sect_ppc(t_env *env, uint32_t (*indian)(uint32_t))
{
	size_t		i;
	size_t		size;

	i = 0;
	size = indian((uint32_t)env->text_size);
	ft_putstr(env, "Contents of (__TEXT,__text) section\n", NO_PROT_RD);
	while (i < size)
	{
		ft_putval(env, (size_t)env->text_addr + i, OUT_32);
		ft_putstr(env, "\t", NO_PROT_RD);
		if (!(move_ptr(env, env->file,
						env->text_offset + i + env->offset + 16)))
			return ;
		ft_put_16_bytes_ppc(env, (size_t)env->file +
				(size_t)env->text_offset + i + env->offset, size - i);
		i += 16;
	}
}

static void		choose_good_arch(t_env *env,
		uint32_t nb, uint32_t magic, uint32_t i)
{
	if (magic == MH_MAGIC)
	{
		handle_32(env, &same_32);
		print_filename(env, nb, magic, i);
		print_sect_32(env, &same_32);
	}
	if (magic == MH_CIGAM)
	{
		handle_32(env, &indian_32);
		print_filename(env, nb, magic, i);
		print_sect_ppc(env, indian_32);
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
