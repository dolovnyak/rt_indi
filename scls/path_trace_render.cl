static float get_random(unsigned int *seed0, unsigned int *seed1) {

	/* hash the seeds using bitwise AND operations and bitshifts */
	*seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);
	*seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

	unsigned int ires = ((*seed0) << 16) + (*seed1);

	/* use union struct to convert int to float */
	union {
		float f;
		unsigned int ui;
	} res;

	res.ui = (ires & 0x007fffff) | 0x40000000;  /* bitwise AND, bitwise OR */
	return (res.f - 2.0f) / 2.0f;
}

static float3 reflect(float3 i, float3 n)
{
	return (i - 2.0f * dot(i, n) * n);
}

static float3 rShlic2(const float3 I, const float3 N, const float refractive_index)
{
	float r0 = (1.f - refractive_index) / (1.f + refractive_index);
	r0 *= r0;
	float cosI = -(dot(N, I));
	const float n = 1. / refractive_index;
	const float sinT2 = n * n * (1.f - cosI * cosI);
	if (sinT2 > 1.f)
		return reflect(I, N);
	float cosX = sqrt(1.f - sinT2);
	float x = 1.f - cosX;
	float eta = r0 + (1.f - r0) * x * x * x * x * x;
	return (1.f / refractive_index) * I + ((1.f / refractive_index) * cos(eta) - sqrt(1.f - sin(eta) * sin(eta))) * N;

}


static float3 refract(const float3 I, float3 N, float refractive_index)
{
	//if (refractive_index < 1.f - 1e-5f)
	// refractive_index = 1./refractive_index;
	//return rShlic2(I, N, refractive_index);
	//return refract(I, N, 1./refractive_index);
	float n = refractive_index;
	float cosI = -(dot(N, I));
	if (cosI < 0)
		cosI = -cosI;
	else {
		n = 1. / n;
		N = -N;
	}
	float sinT2 = n * n * (1.f - cosI * cosI);

	const float cosT = sqrt(1.f - sinT2);
	return  (n * I + (n * cosI - cosT) * N);
}

static float3 refract3(const float3 I, const float3 N, const float refractive_index)
{
	float cosI = -(dot(N, I));
	float etai = 1.f;
	float etat = refractive_index;
	float3 n = N;
	if (cosI < 0)
		n = -N;
	else
	{
		n = -N;
		float tmp = etai;
		etai = etat;
		etat = tmp;
	}
	float eta = etai / etat;
	float k = 1.f - eta * eta * (1 - cosI * cosI);
	return eta * I + (eta * cosI - sqrt(k)) * n;
}

static float3 refract2(const float3 I, const float3 N, const float refractive_index) { // Snell's law
	float cosi = dot(I, N);
	float etai = 1, etat = refractive_index;
	float3 n = N;
	if (cosi < 0) { // if the ray is inside the object, swap the indices and invert the normal to get the correct result
		cosi = -cosi;
		float tmp;
		tmp = etai;
		etai = etat;
		etat = tmp;
		n = -N;
	}
	float eta = etai / etat;
	float k = 1 - eta*eta*(1 - cosi*cosi);
	return k < 0 ? float3(0,0,0) : I*eta + n*(eta * cosi - sqrt(k));
}

static float3 trace(float3 orig, float3 dir, __global t_object *obj, int count,
			const unsigned int *seed0, const unsigned int *seed1,
			const __global int *texture_w,  const __global int *texture_h,
			const __global int *prev_texture_size,  const __global int *texture, float brightness)
{
	float3 path_color = (float3)(0.0f, 0.0f, 0.0f);
	float3 mask = (float3)(1.0f, 1.0f, 1.0f);
	t_lighting lighting;
	lighting.n = (float3) 0;
	lighting.hit = (float3) 0;
	float3 path_orig = orig, path_dir = dir;
	float mirr = 0.f;

	for (int bounces = 0; bounces < 8; bounces++)
	{
		if(!scene_intersect(path_orig, path_dir, obj, &lighting, count,
				texture_w, texture_h, prev_texture_size, texture))
		{
			path_color += mask * 0.01f;
			break;
		}
		float rand1 = get_random((unsigned int *)seed0, (unsigned int *)seed1) * 2.0f * M_PI_F;
		float rand2 = get_random((unsigned int *)seed1, (unsigned int *)seed0);
		float rand2s = sqrt(rand2);
		lighting.n = dot(lighting.n, path_dir) < 0.0f ? lighting.n : lighting.n * (-1.0f);
		float3 w = lighting.n;
		float3 axis = fabs(w.x) >= 0.0f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
		//float3 axis = (float3)(0.0f, 1.0f, 0.0f);
		float3 u = fast_normalize(cross(axis, w));
		float3 v = cross(w, u);
		float3 newdir = fast_normalize(u * cos(rand1) * rand2s + v * sin(rand1) * rand2s + w * sqrt(1.0f - rand2));
		path_orig = lighting.hit + lighting.n * 3e-3f;

		float part = pow((8.f - bounces + mirr) / 8, 5);		// вклад следующего меньше, кроме зеркала и линзы
		path_color += mask * lighting.mat.emission * part;

		if (lighting.mat.reflection > 0)							// ЗЕРКАЛО
		{
			mirr += 1;
		//	float	a = 0.99f;									// параметр. на сколько мутное стекло. диапазон 0.9f - 1.0f
			newdir = fast_normalize(lighting.mat.reflection * fast_normalize(reflect(path_dir, lighting.n)) + (1.f - lighting.mat.reflection) * newdir);
			mask *= pow(fabs(dot(path_dir, lighting.n)), 0.01f);							// затемнеяет зеркало. чем больше угол между нормалью зеркала и dir и больше параметр, тем темнее
			mask *= 0.9f;										// затемняет зеркало
				//path_color += mask;
		}
		else if (lighting.mat.refraction > 0)						// ЛИНЗА
		{
			newdir = fast_normalize(refract(path_dir, lighting.n, lighting.mat.refraction));
			path_orig = lighting.hit - lighting.n * 3e-1f;
			mirr += 1;
			float	cos_n = fabs(dot(path_dir, lighting.n));
			mask *= pow(cos_n, 0.3f);							// аналогично, как и у зеркала
			mask *= 0.9f;										// аналогично, как и у зеркала
		}
		else if (lighting.mat.sp_ex == 30)						// Источник света
			mask *= dot(newdir, lighting.n);
		else
			mask *= dot(newdir, lighting.n);
		path_dir = newdir;
		mask *= lighting.mat.diffuse_color;
	}
	return path_color * brightness;					// 2.f высветляет всю картинку
}


__kernel void   path_trace_render_aa(
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

	int framenumber = 3;

	orig = (*cam).center;
	color = (float3) 0;
    int N = screen->samples;
	for (int i = -fsaa / 2; i <= fsaa / 2; i++)
	{
		for (int j = -fsaa / 2; j <= fsaa / 2; j++)
		{
			dir = (*screen).v1 * ((float) (tx + i * reverse(fsaa)) / WIDTH - 0.5f) -
				  (*screen).v2 * ((float) (ty + j * reverse(fsaa)) / WIDTH - 0.5f);
			dir = dir - (*screen).center;
			dir = fast_normalize(dir);

			unsigned int seed0 = tx * framenumber % WIDTH + (rands.x * WIDTH / 10);
			unsigned int seed1 = ty * framenumber % HEIGHT + (rands.y * HEIGHT / 10);
			for (int k = 0; k <= N; k++)
			{
				get_random(&seed0, &seed1);
				get_random(&seed1, &seed0);
				color += trace(orig, dir, obj, counter->all_obj, &seed0, &seed1,
						texture_w, texture_h, prev_texture_size, texture, screen->brightness) / N;
			}
		}
	}
	color = color / ((fsaa + 1) * (fsaa + 1));

	data[index] = get_color(color, screen->effects);
}

__kernel void   path_trace_render(
				__global int                *data,
                const __global t_cam        *cam,
                const __global t_screen     *screen,
                const __global t_counter    *counter,
                const __global t_light      *l,
                __global t_object     *obj,
                int2                        rands,
                float                       ambient,
                const __global int          *texture,
                const __global int          *texture_w,
                const __global int          *texture_h,
                const __global int          *prev_texture_size)
{
    int             tx;
    int             ty;
    int             index;
    float3          dir;
    float3          orig;
    float3          color;
    int             N;
    unsigned int    seed0;
    unsigned int    seed1;
    tx = get_global_id(0);
    ty = get_global_id(1);
    index = ty * WIDTH + tx;
    orig = (*cam).center;
    color = (float3) 0.f;
    N = screen->samples;
    dir = (*screen).v1 * ((float) (tx) / WIDTH - 0.5f) -
          (*screen).v2 * ((float) (ty) / WIDTH - 0.5f);
    dir = dir - (*screen).center;
    dir = fast_normalize(dir);
    seed0 = tx * 3 % WIDTH + (rands.x * WIDTH / 10);
    seed1 = ty * 3 % HEIGHT + (rands.y * HEIGHT / 10);
    for (int k = 0; k <= N; k++)
    {
        get_random(&seed0, &seed1);
        get_random(&seed1, &seed0);
        color += trace(orig, dir, obj, counter->all_obj, &seed0, &seed1,
                texture_w, texture_h, prev_texture_size, texture, screen->brightness);
    }
    color = color / N;
    data[index] = get_color(color, screen->effects);
}

