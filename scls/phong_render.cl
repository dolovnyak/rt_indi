#include "./includes/kernel.h"
#include "./includes/rt.h"

static float3	light_shadow(float3 dir, __global t_object *obj,
		const __global t_light *l, t_lighting *lighting,
		const __global t_counter *counter,  const __global int *texture_w,
		 const __global int *texture_h, const __global int *prev_texture_size,
		 const __global int *texture, float ambient)
{
	float		light_dist = 0;
	float3		light_dir = (float3) 0;
	float		a = 0;
	float		b = 0;
	t_lighting	shadow_lighting;
	int			i = 0;
	float3		r = (float3) 0;
	float		e = 0;

	shadow_lighting.n = (float3) 0;
	shadow_lighting.hit = (float3) 0;
	i = 0;
	a = 0.f;
	b = 0.f;
	while (i < counter->l)
	{
		light_dir = fast_normalize(l[i].center - lighting->hit);
		light_dist = length(l[i].center - lighting->hit);
		if (dot(light_dir, lighting->n) > 0)
		{
			if (!scene_intersect(l[i].center, -light_dir, obj, &shadow_lighting, counter->all_obj,
					texture_w, texture_h, prev_texture_size, texture) ||
					(length(shadow_lighting.hit - l[i].center) > light_dist - 1e-1f &&
					length(shadow_lighting.hit - l[i].center) < light_dist + 1e-1f))
			{
				a += dot(light_dir, lighting->n) * l[i].intens;
				b += pow(max(0.f, -dot(lighting->n * 2.f * dot(light_dir, lighting->n) - light_dir, dir)), lighting->mat.sp_ex) * l[i].intens;
			}
		}
		i++;
	}
	r = lighting->mat.diffuse_color * (a + ambient) * lighting->mat.al.x + (float3)(1) * lighting->mat.al.y * b;
	e = max(max(r.x, r.y), r.z);
	if (e > 1.f)
		return (r * (1.f / e));
	return (r);
}

__kernel void	phong_render(
				__global int				*data,
				const __global t_cam		*cam,
				const __global t_screen		*screen,
				const __global t_counter	*counter,
				const __global t_light		*l,
				__global t_object		*obj,
				int2 rands, float ambient,
				const __global int *texture, const __global int *texture_w,
				const __global int *texture_h,
				const __global int *prev_texture_size)
{
	int tx = get_global_id(0);
	int ty = get_global_id(1);
	int index = ty * WIDTH + tx;
	float3	dir;
	float3	orig;
	float3 color;
	int fsaa = screen->fsaa_n;

	orig = (*cam).center;
	color = (float3) 0;
	for (int i = -fsaa / 2; i <= fsaa / 2; i++)
	{
		for (int j = -fsaa / 2; j <= fsaa / 2; j++)
		{
			t_lighting	lighting;

			dir = (*screen).v1 * ((float) (tx + i * reverse(fsaa)) / WIDTH - 0.5f) -
				  (*screen).v2 * ((float) (ty + j * reverse(fsaa)) / WIDTH - 0.5f);
			dir = dir - (*screen).center;
			dir = fast_normalize(dir);
			lighting.n = (float3) 0;
			lighting.hit = (float3) 0;

			if(scene_intersect(orig, dir, obj, &lighting, counter->all_obj,
					texture_w, texture_h, prev_texture_size, texture))
				color += light_shadow(dir, obj, l, &lighting, counter,
						texture_w, texture_h, prev_texture_size, texture, ambient);
			else
				color += (float3)(1.f);
		}
	}
	color = color / ((fsaa + 1) * (fsaa + 1));

	data[index] = get_color(color, screen->effects);
}
