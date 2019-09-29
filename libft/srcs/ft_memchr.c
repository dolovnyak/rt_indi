/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 17:56:23 by broggo            #+#    #+#             */
/*   Updated: 2018/11/22 17:56:24 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned const char		*d;

	d = (unsigned const char*)s;
	while (n--)
	{
		if (*d++ == (unsigned char)c)
		{
			d--;
			return ((void *)d);
		}
	}
	return (0);
}
