/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:26:18 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:26:18 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void			print_object_name(t_env *env, char *name)
{
	handle_buff(env, '\n');
	ft_putstr(env, env->filename, NO_PROT_RD);
	handle_buff(env, '(');
	ft_putstr(env, name, PROT_RD);
	ft_putstr(env, "):\n", NO_PROT_RD);
}

static void		print_node(t_env *env, t_symb *node, char type)
{
	(void)env;
	if (node->val || (node->symb != 'U' && node->symb != 'u'))
	{
		if (type == OUT_32)
			ft_putval(env, node->val, OUT_32);
		else if (type == OUT_64)
			ft_putval(env, node->val, OUT_64);
	}
	else
	{
		if (type == 1)
			ft_putstr(env, "         ", NO_PROT_RD);
		else if (type == 2)
			ft_putstr(env, "                 ", NO_PROT_RD);
	}
	handle_buff(env, node->symb);
	ft_putstr(env, " ", NO_PROT_RD);
	ft_putstr(env, node->name, NO_PROT_RD);
	ft_putstr(env, "\n", NO_PROT_RD);
}

void			print_list(t_env *env, t_symb *current, char type)
{
	if (current)
	{
		print_list(env, current->left, type);
		print_node(env, current, type);
		print_list(env, current->right, type);
	}
}

void			print_filename(t_env *env, uint32_t nb,
		uint32_t magic, uint32_t index)
{
	if (nb == 1)
	{
		ft_putstr(env, env->filename, NO_PROT_RD);
		ft_putstr(env, ":\n", NO_PROT_RD);
	}
	else
	{
		handle_buff(env, '\n');
		ft_putstr(env, env->filename, NO_PROT_RD);
		if (magic == MH_MAGIC)
			ft_putstr(env, " (for architecture i386):\n", NO_PROT_RD);
		else
		{
			if (find_cpu_subtype_arch(env, index))
				ft_putstr(env, " (for architecture ppc):\n", NO_PROT_RD);
			else
				ft_putstr(env, " (for architecture ):\n", NO_PROT_RD);
		}
	}
}
