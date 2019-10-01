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

//#include <opencl-c.h>

#ifndef KERNEL_H
# define KERNEL_H

#include "rt.h"

typedef struct	t_lighting
{
	float3		n;
	float3		hit;
	t_material	mat;
}				t_lighting;

float	reverse(int n);
int 	scene_intersect(float3 orig, float3 dir, const __global t_object *obj,
		t_lighting *lighting, int count, __global int *texture_w, __global int *texture_h,
		__global int *prev_texture_size, __global int *texture);
int		ft_sign(float a);
int		cone_intersect(float3 orig, float3 dir, __global t_object *p, float *t0);
int		cyl_intersect(float3 orig, float3 dir, __global t_object *p, float *t0);
int		plane_intersect(float3 orig, float3 dir, __global t_object *p, float *t0);
int		sphere_intersect(float3 orig, float3 dir, __global t_object *s, float *t0);
int		get_light(int start, int end, float percentage);


#endif
