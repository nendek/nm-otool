/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indian_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:24:49 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:25:00 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint32_t		same_32(uint32_t val)
{
	return (val);
}

uint64_t		same_64(uint64_t val)
{
	return (val);
}

uint32_t		indian_32(uint32_t val)
{
	uint32_t	ret;

	ret = 0;
	ret |= (val & 0XFF000000) >> 24;
	ret |= (val & 0XFF0000) >> 8;
	ret |= (val & 0XFF00) << 8;
	ret |= (val & 0XFF) << 24;
	return (ret);
}

uint64_t		indian_64(uint64_t val)
{
	uint64_t	ret;

	ret = 0;
	ret |= (val & 0XFF00000000000000) >> 56;
	ret |= (val & 0XFF000000000000) >> 40;
	ret |= (val & 0XFF0000000000) >> 24;
	ret |= (val & 0XFF00000000) >> 8;
	ret |= (val & 0XFF000000) << 8;
	ret |= (val & 0XFF0000) << 24;
	ret |= (val & 0XFF00) << 40;
	ret |= (val & 0XFF) << 56;
	return (ret);
}
