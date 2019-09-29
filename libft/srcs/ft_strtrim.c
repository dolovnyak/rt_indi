/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 17:13:45 by broggo            #+#    #+#             */
/*   Updated: 2018/11/27 17:13:46 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check2(char c)
{
	if (c == '\n' || c == '\t' || c == ' ')
		return (1);
	return (0);
}

char		*ft_strtrim(char const *s)
{
	char			*s2;
	size_t			len1;
	unsigned int	start;

	if (!s)
		return (0);
	len1 = ft_strlen(s);
	start = 0;
	while (check2(*(s + start)))
		start++;
	if (start == len1)
	{
		s2 = (char *)malloc((1) * sizeof(char));
		*s2 = '\0';
		return (s2);
	}
	while (check2(*(s + len1 - 1)))
		len1--;
	s2 = ft_strsub(s, start, len1 - start);
	return (s2);
}
