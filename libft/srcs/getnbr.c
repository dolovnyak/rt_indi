/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getnbr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:21:19 by broggo            #+#    #+#             */
/*   Updated: 2019/03/01 18:21:20 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	getnbr(char *str, int *err)
{
	unsigned int	numb;
	int				i;
	int				m;

	i = -1;
	m = 1;
	if (*str == '-')
	{
		str++;
		m = -1;
	}
	else if (*str == '+')
		str++;
	numb = 0;
	while (*(str + ++i) != '\0')
	{
		if (!ft_isdigit(*(str + i)))
		{
			*err = 1;
			return (0);
		}
		numb = numb * 10 + *(str + i) - '0';
	}
	return (m * numb);
}
