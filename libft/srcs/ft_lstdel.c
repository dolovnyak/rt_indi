/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 13:27:40 by broggo            #+#    #+#             */
/*   Updated: 2018/11/28 13:27:40 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list *a;

	a = *alst;
	if (!alst || !*alst || !del)
		return ;
	while (*alst)
	{
		a = *alst;
		del((*alst)->content, (*alst)->content_size);
		*alst = (*alst)->next;
		free(a);
	}
	*alst = NULL;
}
