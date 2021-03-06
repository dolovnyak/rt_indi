/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jtoc_get_raw_data.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbednar <sbednar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 20:24:00 by sbednar           #+#    #+#             */
/*   Updated: 2019/08/19 04:03:37 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libjtoc.h"

static size_t	get_tree_size(t_jnode *n)
{
	t_jnode	*children;
	char	contains_ptrs;
	size_t	res;

	children = n->down;
	contains_ptrs = 0;
	res = 0;
	while (children)
	{
		if (children->type == integer || children->type == fractional)
			res += 4;
		else
		{
			if (res % 8 != 0)
				res += 4;
			res += 8;
			contains_ptrs = 1;
		}
		children = children->right;
	}
	if (contains_ptrs && res % 8 != 0)
		res += 4;
	return (res);
}

static void		*get_string(t_jnode *n)
{
	size_t	len;
	void	*src;
	void	*res;

	src = jtoc_get_string(n);
	len = ft_strlen(src);
	if (!(res = malloc(len + 1)))
		return (NULL);
	ft_memcpy(res, src, len + 1);
	return (res);
}

static int		get_array_size(t_jnode *n)
{
	int		act;
	int		tmp;
	t_jnode	*cur;

	act = -1;
	cur = n->down;
	if (!cur)
		return (-1);
	while (cur)
	{
		tmp = get_tree_size(cur);
		if (act < 0)
			act = tmp;
		else if (act != tmp)
			return (-1);
		cur = cur->right;
	}
	return (act * jtoc_get_child_count(n));
}

static void		*get_array(t_jnode *n)
{
	char	*res;
	void	*vres;
	void	*tmp;
	int		size;
	int		ssize;

	if ((size = get_array_size(n)) < 0 ||
		!(res = malloc(size)))
		return (NULL);
	ssize = get_tree_size(n->down);
	vres = (void *)res;
	n = n->down;
	while (n)
	{
		if (!(tmp = jtoc_get_raw_data(n)))
		{
			free(res);
			return (NULL);
		}
		ft_memcpy(res, &tmp, ssize);
		n = n->right;
		res += ssize;
	}
	return (vres);
}

void			*jtoc_get_raw_data(t_jnode *n)
{
	size_t		size;
	char		*res;
	size_t		i;
	t_jnode		*cur;
	void		*tmp;

	size = get_tree_size(n);
	if (!(res = (char *)malloc(size)))
		return (NULL);
	i = 0;
	cur = n->down;
	while (cur)
	{
		if (i != 0 && cur->type != integer &&
				cur->type != fractional && i % 8 != 0)
			i += 4;
		if (cur->type == integer || cur->type == fractional)
			ft_memcpy(res + i, cur->data, 4);
		else if (jtoc_get_raw_data_help(cur, res, &tmp, &i))
			return (NULL);
		cur = cur->right;
		i += 4;
	}
	return ((void *)res);
}
