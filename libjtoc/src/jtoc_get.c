/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jtoc_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbednar <sbednar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 19:04:36 by sbednar           #+#    #+#             */
/*   Updated: 2019/09/29 21:18:33 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libjtoc.h"

int		jtoc_get_int(t_jnode *n)
{
	return (*((int *)n->data));
}

float	jtoc_get_float(t_jnode *n)
{
	return (*((float *)n->data));
}

char	*jtoc_get_string(t_jnode *n)
{
	return ((char *)n->data);
}