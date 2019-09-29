/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float3_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 14:42:17 by broggo            #+#    #+#             */
/*   Updated: 2019/08/05 14:42:17 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3	cl_normalize(cl_float3 v1)
{
	cl_float3	v;
	float		l;
	
	l = cl_length(v1);
	v.x = v1.x / l;
	v.y = v1.y / l;
	v.z = v1.z / l;
	return (v);
}

cl_float3	cl_cross(cl_float3 v1, cl_float3 v2)
{
	cl_float3	v;
	
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return (v);
}

float    cl_dot(cl_float3 v1, cl_float3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float   cl_angle(cl_float3 v1, cl_float3 v2)
{
	float   angle;
	float   l1;
	float   l2;
	float   dot;
	
	l1 = cl_length(v1);
	l2 = cl_length(v2);
	dot = cl_dot(v1, v2) / (l1 * l2);
	if (dot > 1.f - 1e-5f)
		return (0.f);
	else if (dot < 1e-5f)
		return ((float)M_PI);
	angle = acosf(dot);
	return (angle);
}


