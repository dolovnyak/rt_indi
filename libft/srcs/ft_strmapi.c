/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 16:08:56 by broggo            #+#    #+#             */
/*   Updated: 2018/11/27 16:08:56 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len;
	char			*s2;
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	s2 = (char *)malloc((len + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	while (*(s + i))
	{
		*(s2 + i) = f(i, *(s + i));
		i++;
	}
	s2[len] = '\0';
	return (s2);
}
