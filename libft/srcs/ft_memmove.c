/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 17:21:52 by broggo            #+#    #+#             */
/*   Updated: 2018/11/22 17:21:57 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char*)dst;
	s = (const unsigned char*)src;
	if (d <= s || d >= (s + len))
	{
		while (len--)
			*d++ = *s++;
	}
	else
	{
		d += len - 1;
		s += len - 1;
		while (len--)
			*d-- = *s--;
	}
	return (dst);
}
