/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putval.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:29:09 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:29:11 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static char		get_size(size_t val)
{
	if (val < 16)
		return (1);
	else
		return (get_size(val / 16) + 1);
}

char			get_char(size_t nb)
{
	return ((char)nb + (nb < 10 ? '0' : 87));
}

static void		ft_putnbr(t_env *env, size_t val)
{
	if (val < 16)
		handle_buff(env, get_char(val));
	else
	{
		ft_putnbr(env, val / 16);
		ft_putnbr(env, val % 16);
	}
}

void			ft_putval(t_env *env, size_t val, char type)
{
	char size;

	size = get_size(val);
	if (type == OUT_32)
	{
		while (size < 8)
		{
			handle_buff(env, '0');
			size++;
		}
		ft_putnbr(env, val);
	}
	else
	{
		while (size < 16)
		{
			handle_buff(env, '0');
			size++;
		}
		ft_putnbr(env, val);
	}
}
