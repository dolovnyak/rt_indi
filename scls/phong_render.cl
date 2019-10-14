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
	int 		counter_l = counter->l;


	shadow_lighting.n = (float3) 0;
	shadow_lighting.hit = (float3) 0;
	i = 0;
	a = 0.f;
	b = 0.f;
	while (i < counter_l)
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
	int		tx;
	int		ty;
	int		index;
	float3	dir;
	float3	orig;
	float3	color;
	int		fsaa;
	float		cache_width;
	t_lighting	lighting;

	tx = get_global_id(0);
	ty = get_global_id(1);
	index = ty * WIDTH + tx;
	orig = (*cam).center;
	color = (float3) 0;
	fsaa = screen->fsaa_n;

	cache_width = 1.f / WIDTH;
	for (int i = -fsaa * 0.5f; i <= fsaa * 0.5f; i++)
	{
		for (int j = -fsaa * 0.5f; j <= fsaa * 0.5f; j++)
		{
			dir = (*screen).v1 * ((float) (tx + i * reverse(fsaa)) * cache_width - 0.5f) -
				  (*screen).v2 * ((float) (ty + j * reverse(fsaa)) * cache_width - 0.5f);
			dir = dir - (*screen).center;
			dir = fast_normalize(dir);

			if (scene_intersect(orig, dir, obj, &lighting, counter->all_obj,
					texture_w, texture_h, prev_texture_size, texture))
				color += light_shadow(dir, obj, l, &lighting, counter,
						texture_w, texture_h, prev_texture_size, texture, ambient);
			else if (screen->skybox_id != -1)
			{
				float3	vec;
				float 	v;
				float 	u;

				vec = -dir;
				u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
				v = 0.5f + (asin(vec.z) / M_PI_F);
				float2 uv = (float2)(u, v);

				int	coord;
				int coord_x;
				int coord_y;
				float3 color_uv;

				coord_x = (int)((uv.x * texture_w[screen->skybox_id]));
				coord_y = (int)((uv.y * texture_h[screen->skybox_id]));
				coord_y *= (texture_w[screen->skybox_id]);
				coord = coord_x + coord_y;
				coord += prev_texture_size[screen->skybox_id];

				color_uv.z = (RED(texture[coord]));
				color_uv.y = (GREEN(texture[coord]));
				color_uv.x = (BLUE(texture[coord]));
				color_uv.x *= 0.00392156862f;
				color_uv.y *= 0.00392156862f;
				color_uv.z *= 0.00392156862f;
				color += color_uv;
			}
			else
				color += (float3)(0.f);
		}
	}
	color = color / ((fsaa + 1) * (fsaa + 1));
	data[index] = get_color(color, screen->effects);
}
