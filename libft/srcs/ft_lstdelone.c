/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 13:27:32 by broggo            #+#    #+#             */
/*   Updated: 2018/11/28 13:27:33 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list **alst, void (*del)(void *, size_t))
{
	t_list *a;

	if (!alst || !*alst || !del)
		return ;
	a = (*alst)->next;
	del((*alst)->content, (*alst)->content_size);
	free(*alst);
	*alst = NULL;
}
