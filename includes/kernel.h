/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 13:54:37 by broggo            #+#    #+#             */
/*   Updated: 2019/09/29 22:53:21 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

#include "rt.h"

typedef struct	t_lighting
{
	float3		n;
	float3		hit;
	t_material	mat;
}				t_lighting;

#endif
