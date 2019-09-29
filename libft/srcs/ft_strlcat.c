/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 17:38:09 by broggo            #+#    #+#             */
/*   Updated: 2018/11/21 17:38:10 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	while (*(dst + i))
		i++;
	while (*(src + k))
		k++;
	if (size <= i)
		return (size + k);
	while ((*(src + j)) && (j + i < (size - 1)))
	{
		*(dst + i + j) = *(src + j);
		j++;
	}
	*(dst + i + j) = '\0';
	return (i + k);
}
