/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_x_memalloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 21:47:16 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/24 21:47:17 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_x_memalloc(size_t size)
{
	void	*res;

	if (!(res = malloc(size)))
		exit(-1);
	ft_bzero(res, size);
	return (res);
}
