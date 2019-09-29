/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 17:49:43 by broggo            #+#    #+#             */
/*   Updated: 2018/11/21 17:49:44 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	len_find;

	if (*needle == '\0')
		return ((char *)haystack);
	len_find = ft_strlen(needle);
	while (*haystack != '\0' && len-- >= len_find)
	{
		if (*haystack == *needle && ft_memcmp(haystack, needle, len_find) == 0)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
