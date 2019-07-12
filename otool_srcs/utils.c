/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:29:27 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:29:28 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void			print_buff(t_env *env)
{
	write(1, env->print_buff, env->index_buff);
	env->index_buff = 0;
}

void			handle_buff(t_env *env, char c)
{
	env->print_buff[env->index_buff] = c;
	env->index_buff++;
	if (env->index_buff == 4095)
		print_buff(env);
}

char			*get_name(t_env *env, char *string, char prot)
{
	char	*ret;
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(env, string, prot);
	if (!(ret = malloc(sizeof(char) * len + 1)))
		return (NULL);
	while (i < len)
	{
		ret[i] = string[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void			*move_ptr(t_env *env, void *ptr, size_t to_add)
{
	if ((size_t)ptr + to_add >= (size_t)env->endfile)
	{
		write(2, "Error, corruption detected\n", 27);
		return (NULL);
	}
	return ((void *)((size_t)ptr + to_add));
}
