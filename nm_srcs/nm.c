/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:25:48 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:25:49 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		free_tree(t_symb *node)
{
	if (node)
	{
		free_tree(node->left);
		free_tree(node->right);
		free(node->name);
		free(node);
		node = NULL;
	}
}

static void	free_all(t_env *env)
{
	free((env->filename));
	free((env->print_buff));
	free_tree(env->lst);
	env->lst = NULL;
	free(env);
}

static void	choose_good_func(t_env *env, uint32_t magic_number)
{
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(env, &same_32);
		print_list(env, env->lst, OUT_64);
	}
	else if (magic_number == MH_MAGIC)
	{
		handle_32(env, &same_32);
		print_list(env, env->lst, OUT_32);
	}
	else if (magic_number == MH_CIGAM)
	{
		handle_32(env, &indian_32);
		print_list(env, env->lst, OUT_32);
	}
	else if (magic_number == MH_CIGAM_64)
	{
		handle_64(env, &indian_32);
		print_list(env, env->lst, OUT_64);
	}
	else if (magic_number == FAT_CIGAM)
		handle_fat_cigam(env);
	else
		handle_lib(env, 0);
}

void		nm(void *file, char *filename, size_t size_file)
{
	uint32_t	magic_number;
	t_env		*env;

	if (!(env = init_env(file, filename, size_file)))
		return ;
	magic_number = *(uint32_t *)file;
	choose_good_func(env, magic_number);
	print_buff(env);
	free_all(env);
}

int			main(int argc, char **argv)
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
	nm(file, argv[1], (size_t)buf.st_size);
	if (munmap(file, (size_t)buf.st_size) < 0)
	{
		close(fd);
		return (EXIT_FAILURE);
	}
	close(fd);
	return (EXIT_SUCCESS);
}
