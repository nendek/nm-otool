/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:29:19 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:29:19 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void		ft_putstr(t_env *env, char *str, char prot)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		handle_buff(env, str[i]);
		if (prot == PROT_RD)
			if (!(move_ptr(env, str, i + 1)))
				return ;
		i++;
	}
}

int			ft_strcmp(t_env *env, char *s1, char *s2)
{
	int				i;
	unsigned char	*m1;
	unsigned char	*m2;

	m1 = (unsigned char*)s1;
	m2 = (unsigned char*)s2;
	i = 0;
	(void)env;
	while (m1[i] == m2[i] && m1[i] != '\0' && m2[i] != '\0')
		i++;
	return ((m1[i] - m2[i]));
}

size_t		ft_strlen(t_env *env, char *string, char prot)
{
	size_t	ret;

	ret = 0;
	while (*string)
	{
		ret++;
		if (prot == PROT_RD)
		{
			if (!(move_ptr(env, string, 1)))
				return (ret);
		}
		string++;
	}
	return (ret);
}

size_t		get_name_len(t_env *env, char print)
{
	char	*name;

	if (!(move_ptr(env, env->file, env->offset + 3)))
		return (0);
	name = (char *)((size_t)env->file + env->offset);
	if (name[0] == '#' && name[1] == '1' && name[2] == '/')
	{
		if (print == PRINT)
			print_object_name(env, (char *)(move_ptr(env, name, 60)));
		return (ft_atoi(env, &(name[3])));
	}
	else
	{
		if (print == PRINT)
			print_object_name(env, name);
		return (0);
	}
}
