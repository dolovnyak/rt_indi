/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 18:04:41 by broggo            #+#    #+#             */
/*   Updated: 2018/11/21 18:04:41 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	int		k;

	i = 0;
	k = -1;
	while (*(s + i))
	{
		if (*(s + i) == c)
			k = (int)i;
		i++;
	}
	if (!c)
		return ((char *)s + i);
	if (k >= 0)
		return ((char *)s + k);
	return (0);
}
