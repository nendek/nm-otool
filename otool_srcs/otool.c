/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:28:34 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:28:35 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int		is_lib(t_env *env)
{
	size_t magic;

	magic = *(size_t *)(env->file);
	if (magic == (LIB_MAGIC))
		return (1);
	return (0);
}

static void		choose_good_func_2(t_env *env, uint32_t magic_number)
{
	if (magic_number == MH_CIGAM_64)
	{
		ft_putstr(env, env->filename, NO_PROT_RD);
		ft_putstr(env, ":\n", NO_PROT_RD);
		handle_64(env, &indian_32);
		print_sect_64(env, &indian_32);
	}
	else if (magic_number == FAT_CIGAM)
		handle_fat_cigam(env);
	else if (is_lib(env) == 1)
	{
		ft_putstr(env, "Archive : ", NO_PROT_RD);
		ft_putstr(env, env->filename, NO_PROT_RD);
		handle_buff(env, '\n');
		handle_lib(env, 0);
	}
	else
	{
		ft_putstr(env, env->filename, NO_PROT_RD);
		ft_putstr(env, ": is not an object file\n", NO_PROT_RD);
	}
}

static void		choose_good_func(t_env *env, uint32_t magic_number)
{
	if (magic_number == MH_MAGIC_64)
	{
		ft_putstr(env, env->filename, NO_PROT_RD);
		ft_putstr(env, ":\n", NO_PROT_RD);
		handle_64(env, &same_32);
		print_sect_64(env, &same_32);
	}
	else if (magic_number == MH_MAGIC)
	{
		ft_putstr(env, env->filename, NO_PROT_RD);
		ft_putstr(env, ":\n", NO_PROT_RD);
		handle_32(env, &same_32);
		print_sect_32(env, &same_32);
	}
	else if (magic_number == MH_CIGAM)
	{
		ft_putstr(env, env->filename, NO_PROT_RD);
		ft_putstr(env, ":\n", NO_PROT_RD);
		handle_32(env, &indian_32);
		print_sect_ppc(env, &indian_32);
	}
	else
		choose_good_func_2(env, magic_number);
}

void			otool(void *file, char *filename, size_t size_file)
{
	uint32_t	magic_number;
	t_env		*env;

	if (!(env = init_env(file, filename, size_file)))
		return ;
	magic_number = *(uint32_t *)file;
	choose_good_func(env, magic_number);
	print_buff(env);
	free_env(env);
}

int				main(int argc, char **argv)
{
	void			*file;
	struct stat		buf;
	int				fd;

	if (argc != 2)
		return (EXIT_FAILURE);
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		return (EXIT_FAILURE);
	if (fstat(fd, &buf) < 0)
	{
		close(fd);
		return (EXIT_FAILURE);
	}
	if ((file = mmap(0, (size_t)buf.st_size, PROT_READ,
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (EXIT_FAILURE);
	otool(file, argv[1], (size_t)buf.st_size);
	if (munmap(file, (size_t)buf.st_size) < 0)
	{
		close(fd);
		return (EXIT_FAILURE);
	}
	close(fd);
	return (EXIT_SUCCESS);
}
