/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 13:28:06 by broggo            #+#    #+#             */
/*   Updated: 2018/11/28 13:28:06 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*a;
	t_list	*b;

	if (!lst)
		return (NULL);
	a = f(lst);
	b = a;
	while (lst->next)
	{
		lst = lst->next;
		if (!(a->next = f(lst)))
		{
			free(a->next);
			return (NULL);
		}
		a = a->next;
	}
	return (b);
}
