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

#define WIDTH 		1280
#define HEIGHT		1024
#define	MAX_DIST	10000.f
#define	SIGMA		3

typedef struct	s_material
{
	float3		diffuse_color;
	float2		al;
	float		sp_ex;
	float3		emission;
    int         texture_id;
}				t_material;

typedef struct	s_object
{
	int			type;
	float3		center;
	float3		vector;
	float		radius;
	float		param;
	float3		a;
	float3		b;
	float3		c;
	float3		d;
	t_material	mat;
}				t_object;

typedef struct	s_cam
{
#ifndef OPENCL___
	cl_float3		center;
	cl_float		alpha;
	cl_float		betta;
#else
	float3		center;
	float		alpha;
	float		betta;
#endif
}				t_cam;

typedef struct	s_light
{
	float3		center;
	float		intens;
}				t_light;

typedef struct	s_counter
{
	int			s;
	int			l;
	int			all_obj;
}				t_counter;

typedef struct	s_screen
{
	float3		v1;
	float3		v2;
	float3		center;
	int			fsaa_n;
	int8		effects;
}				t_screen;

typedef struct	t_lighting
{
	float3		n;
	float3		hit;
	t_material	mat;
}				t_lighting;




__kernel void	display(
						__global int				*data,
						const __global t_cam		*cam,
						const __global t_screen		*screen,
						const __global t_counter	*counter,
						const __global t_light		*l,
						const __global t_object		*obj
						);
float	reverse(int n);
float3	light_shadow(float3 dir, const __global t_object *obj, const __global t_light *l, t_lighting *lighting, const __global t_counter *counter);
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
