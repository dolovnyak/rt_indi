/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 15:56:58 by broggo            #+#    #+#             */
/*   Updated: 2018/11/27 15:56:58 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t	len;
	char	*s2;

	if (!s || !f)
		return (NULL);
	len = 0;
	while (*(s + len))
		len++;
	s2 = (char *)malloc((len + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	s2[len] = '\0';
	while (len)
	{
		*(s2 + len - 1) = f(*(s + len - 1));
		len--;
	}
	return (s2);
}
