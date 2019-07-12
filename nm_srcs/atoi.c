/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:22:06 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:22:08 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char		ft_iswp(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' \
			|| c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

static char		ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

size_t			ft_atoi(t_env *env, char *str)
{
	int			i;
	size_t		res;

	i = 0;
	res = 0;
	while (str[i] != '\0' && ft_iswp(str[i]) == 1)
	{
		if (!(move_ptr(env, str, (size_t)i + 1)))
			return (0);
		i++;
	}
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) != 1)
			return (res);
		res = res * 10 + (size_t)(str[i] - '0');
		if (!(move_ptr(env, str, (size_t)i + 1)))
			return (0);
		i++;
	}
	return (res);
}
