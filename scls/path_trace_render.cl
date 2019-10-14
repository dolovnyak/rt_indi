#include "./includes/kernel.h"
#include "./includes/rt.h"

static float get_random(unsigned int *seed0, unsigned int *seed1) {

	*seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);
	*seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

	unsigned int		ires = ((*seed0) << 16) + (*seed1);
	union {
		float			f;
		unsigned int	ui;
	} res;

	res.ui = (ires & 0x007fffff) | 0x40000000;
	return ((res.f - 2.0f) / 2.0f);
}

static float3 reflect(float3 i, float3 n)
{
	return (i - 2.0f * dot(i, n) * n);
}

static float3 refract(const float3 I, float3 N, float refractive_index)
{
	float		n = refractive_index;
	float		cosI = -(dot(N, I));

	if (cosI < 0.f)
		cosI = -cosI;
	else
	{
		n = 1.f / n;
		N = -N;
	}
	float		sinT2 = n * n * (1.f - cosI * cosI);
	const float	cosT = sqrt(1.f - sinT2);
	return  (n * I + (n * cosI - cosT) * N);
}

static float3 trace(float3 orig, float3 dir, __global t_object *obj, int count,
			const unsigned int *seed0, const unsigned int *seed1,
			const __global int *texture_w,  const __global int *texture_h,
			const __global int *prev_texture_size,  const __global int *texture,
			float brightness, int skybox_id)
{
	float3		path_color = (float3)(0.0f, 0.0f, 0.0f);
	float3		mask = (float3)(1.0f, 1.0f, 1.0f);
	t_lighting	lighting;
	float3		path_orig = orig;
	float3		path_dir = dir;
	float		mirr = 0.f;

	lighting.n = (float3) 0.f;
	lighting.hit = (float3) 0.f;

	for (int bounces = 0; bounces < 8; bounces++)
	{
		if (!scene_intersect(path_orig, path_dir, obj, &lighting, count,
				texture_w, texture_h, prev_texture_size, texture))
		{
			if (skybox_id != -1)
			{
				float3 vec;
				float v;
				float u;

				vec = -path_dir;
				u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
				v = 0.5f + (asin(vec.z) / M_PI_F);
				float2 uv = (float2)(u, v);

				int coord;
				int coord_x;
				int coord_y;
				float3 color_uv;

				coord_x = (int) ((uv.x * texture_w[skybox_id]));
				coord_y = (int) ((uv.y * texture_h[skybox_id]));
				coord_y *= (texture_w[skybox_id]);
				coord = coord_x + coord_y;
				coord += prev_texture_size[skybox_id];

				color_uv.
				z = (RED(texture[coord]));
				color_uv.
				y = (GREEN(texture[coord]));
				color_uv.
				x = (BLUE(texture[coord]));
				color_uv.x *= 0.00392156862f;
				color_uv.y *= 0.00392156862f;
				color_uv.z *= 0.00392156862f;
				path_color += mask * 0.2f * color_uv; // 0.2f - like params in json
			}
			else
				path_color += mask * 0.01f;
			break;
		}
		float	rand1 = get_random((unsigned int *)seed0, (unsigned int *)seed1) * 2.0f * M_PI_F;
		float	rand2 = get_random((unsigned int *)seed1, (unsigned int *)seed0);
		float	rand2s = sqrt(rand2);
		lighting.n = dot(lighting.n, path_dir) < 0.0f ? lighting.n : lighting.n * (-1.0f);
		float3	w = lighting.n;
		float3	axis = fabs(w.x) > 1e-5f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
		float3	u = fast_normalize(cross(axis, w));
		float3	v = cross(w, u);
		float3	newdir = fast_normalize(u * cos(rand1) * rand2s + v * sin(rand1) * rand2s + w * sqrt(1.0f - rand2));
		path_orig = lighting.hit + lighting.n * 3e-3f;

		float	part = pow((8.f - bounces + mirr) * 0.125f, 5);
		path_color += mask * lighting.mat.emission * part;

		if (lighting.mat.reflection > 0.f)
		{
			mirr += 1;
			newdir = fast_normalize(lighting.mat.reflection * fast_normalize(reflect(path_dir, lighting.n)) + (1.f - lighting.mat.reflection) * newdir);
			mask *= pow(fabs(dot(path_dir, lighting.n)), 0.01f);
			mask *= 0.95f;
		}
		else if (lighting.mat.refraction > 0.f)
		{
			newdir = fast_normalize(refract(path_dir, lighting.n, lighting.mat.refraction));
			path_orig = lighting.hit - lighting.n * 3e-1f;
			mirr += 1;
			float	cos_n = fabs(dot(path_dir, lighting.n));
			mask *= pow(cos_n, 0.3f);
			mask *= 0.95f;
		}
		else
			mask *= dot(newdir, lighting.n);
		path_dir = newdir;
		mask *= lighting.mat.diffuse_color;
	}
	return (path_color * brightness);
}

__kernel void   path_trace_render(
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
	int             tx;
	int             ty;
	int             index;
	float3          dir;
	float3          orig;
	float3          color;
	int             N;
	int				fsaa;
	unsigned		seed0, seed1;

	tx = get_global_id(0);
	ty = get_global_id(1);
	index = ty * WIDTH + tx;
	orig = (*cam).center;
	color = (float3) 0.f;
	N = screen->samples;
	fsaa = screen->fsaa_n;
	for (int i = -fsaa * 0.5f; i <= fsaa * 0.5f; i++)
	{
		for (int j = -fsaa * 0.5f; j <= fsaa * 0.5f; j++)
		{
			dir = (*screen).v1 * ((float) (tx + i * reverse(fsaa)) / WIDTH - 0.5f) -
				  (*screen).v2 * ((float) (ty + j * reverse(fsaa)) / WIDTH - 0.5f);
			dir = dir - (*screen).center;
			dir = fast_normalize(dir);

			seed0 = tx * 3 % WIDTH + (rands.x * (int)(WIDTH * 0.1f));
			seed1 = ty * 3 % HEIGHT + (rands.y * (int)(HEIGHT * 0.1f));
			for (int k = 0; k <= N; k++)
			{
				get_random(&seed0, &seed1);
				get_random(&seed1, &seed0);
				color += trace(orig, dir, obj, counter->all_obj, &seed0, &seed1,
						texture_w, texture_h, prev_texture_size, texture,
						screen->brightness, screen->skybox_id);
			}
		}
	}
	color = color / N;
	color = color / ((fsaa + 1) * (fsaa + 1));

	data[index] = get_color(color, screen->effects);
}
