/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:20:59 by broggo            #+#    #+#             */
/*   Updated: 2018/11/22 18:21:00 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t				i;
	unsigned char		*d;
	unsigned const char	*s;

	i = 0;
	d = (unsigned char *)dst;
	s = (unsigned const char *)src;
	while (i < n)
	{
		*(d + i) = *(s + i);
		if (*(d + i) == (unsigned char)c)
		{
			i++;
			return (dst + i);
		}
		i++;
	}
	return (0);
}
