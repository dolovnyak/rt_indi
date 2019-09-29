/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:04:02 by broggo            #+#    #+#             */
/*   Updated: 2018/11/22 18:04:02 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char*)s1;
	s = (const unsigned char*)s2;
	while (n)
	{
		if (*d != *s)
			return (*d - *s);
		d++;
		s++;
		n--;
	}
	return (0);
}
