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

#  define RED(c)			(((int)c >> 16) & 0xFF)
#  define GREEN(c)			(((int)c >> 8) & 0xFF)
#  define BLUE(c)			((int)c & 0xFF)

typedef struct	t_lighting
{
	float3		n;
	float3		hit;
	t_material	mat;
}				t_lighting;

int		scene_intersect(float3 orig, float3 dir, __global t_object *obj,
		t_lighting *lighting, int count, const __global int *texture_w,
		const __global int *texture_h, const __global int *prev_texture_size,
		const __global int *texture);

//uv_mapping
float2	uv_mapping_for_sphere(t_lighting *lighting);
float2	uv_mapping_for_cylinder(t_lighting *lighting, __global t_object *obj);
float2	uv_mapping_for_torus(t_lighting *lighting, __global t_object *obj);
float2	uv_mapping_for_plane(t_lighting *lighting);
float2	uv_mapping_for_cone(t_lighting *lighting, __global t_object *obj);

//utilities
float	reverse(int n);
int		get_color(float3 v, int type);
int		get_light(int start, int end, float percentage);

#endif
