/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 17:28:36 by broggo            #+#    #+#             */
/*   Updated: 2018/11/27 17:28:37 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		check(char c, char c1)
{
	if (c == c1)
		return (0);
	if (c == ' ')
		return (0);
	return (1);
}

static size_t	checker(char const *s, char c, size_t *i)
{
	size_t	z;

	while (*(s + *i) && (1 - check(*(s + *i), c)))
		(*i)++;
	z = 0;
	while (*(s + *i) && check(*(s + *i), c))
	{
		(*i) = (*i) + 1;
		z++;
	}
	return (z);
}

static void		ft_arr_del(char **arr, size_t z)
{
	size_t	i;

	i = 0;
	while (i < z)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void		ft_arr(char **arr, char const *s, char c)
{
	size_t i;
	size_t z;
	size_t j;

	i = 0;
	z = 0;
	while (*(s + i))
	{
		j = checker(s, c, &i);
		if (j > 0)
		{
			arr[z] = ft_strsub(s, i - j, j);
			if (arr[z] == NULL)
			{
				ft_arr_del(arr, z);
				return ;
			}
			z++;
		}
	}
}

char			**ft_strsplit(char const *s, char c, size_t *j)
{
	char	**arr;
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	*j = 0;
	while (*(s + i))
		if (checker(s, c, &i) > 0)
			(*j)++;
	if (!(arr = (char **)malloc(sizeof(char*) * (*j + 1))))
		return (NULL);
	*(arr + *j) = 0;
	ft_arr(arr, s, c);
	return (arr);
}
