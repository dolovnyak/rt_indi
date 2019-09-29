/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 13:27:20 by broggo            #+#    #+#             */
/*   Updated: 2018/11/28 13:27:20 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*a;

	if (!(a = malloc(sizeof(t_list))))
		return (NULL);
	if (content == NULL)
	{
		a->content = NULL;
		a->content_size = 0;
		a->next = NULL;
		return (a);
	}
	if (!(a->content = malloc(content_size)))
		return (NULL);
	ft_memcpy((a->content), content, content_size);
	a->content_size = content_size;
	a->next = NULL;
	return (a);
}
