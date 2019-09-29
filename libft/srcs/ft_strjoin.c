/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 17:06:27 by broggo            #+#    #+#             */
/*   Updated: 2018/11/27 17:06:27 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (0);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	while (len1)
	{
		s3[len1 - 1] = s1[len1 - 1];
		len1--;
	}
	len1 = ft_strlen(s1);
	s3[len1 + len2] = '\0';
	while (len2)
	{
		s3[len1 + len2 - 1] = s2[len2 - 1];
		len2--;
	}
	return (s3);
}
