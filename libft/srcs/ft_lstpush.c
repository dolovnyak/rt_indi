/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpush.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:24:04 by broggo            #+#    #+#             */
/*   Updated: 2019/03/05 17:24:04 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_lstpush(t_list **begin_list, void *data, size_t content_size)
{
	t_list	*a;

	if (!begin_list)
		return (1);
	if (*begin_list)
	{
		a = *begin_list;
		while (a->next)
			a = a->next;
		a->next = ft_lstnew(data, content_size);
	}
	else
		*begin_list = ft_lstnew(data, content_size);
	return (0);
}
