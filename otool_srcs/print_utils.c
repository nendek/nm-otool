/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:28:53 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:28:55 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void			print_object_name(t_env *env, char *name)
{
	ft_putstr(env, env->filename, NO_PROT_RD);
	handle_buff(env, '(');
	ft_putstr(env, name, PROT_RD);
	ft_putstr(env, "):\n", NO_PROT_RD);
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
		ft_putstr(env, env->filename, NO_PROT_RD);
		if (magic == MH_MAGIC)
			ft_putstr(env, " (architecture i386):\n", NO_PROT_RD);
		else
		{
			if (find_cpu_subtype_arch(env, index))
				ft_putstr(env, " (architecture ppc):\n", NO_PROT_RD);
			else
				ft_putstr(env, ":\n", NO_PROT_RD);
		}
	}
}
