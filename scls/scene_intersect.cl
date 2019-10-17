#include "./includes/kernel.h"
#include "./includes/rt.h"

static void	normalize_coord_for_texture(__global t_object *obj, float2 uv, float3 *color,
		const __global int *texture,  const __global int *texture_w,
		const __global int *texture_h, const __global int *prev_texture_size)
{
	int	coord;
	int coord_x;
	int coord_y;
	int	start;
	int	end;

	coord_x = (int)((uv.x * texture_w[obj->mat.texture_id]));
	coord_y = (int)((uv.y * texture_h[obj->mat.texture_id]));
	coord_y *= (texture_w[obj->mat.texture_id]);
	coord = coord_x + coord_y;
	coord += prev_texture_size[obj->mat.texture_id];

	start = 0;
	end = 0xFFFFFF;
	color->z = (RED(texture[coord]));
	color->y = (GREEN(texture[coord]));
	color->x = (BLUE(texture[coord]));
	color->x *= 0.00392156862f;
	color->y *= 0.00392156862f;
	color->z *= 0.00392156862f;
}

static int  ft_sign(float a)
{
	if (a > 1e-3)
		return (1);
	if (a < -1e-3)
		return -1;
	return (0);
}

static int torus_intersect(float3 orig, float3 dir, __global t_object *s, float *t0)
{
	int		i, j;
	float3	current_position = (float3) 0;
	float	distance_to_closest = 0.f;
	float2	q = (float2) 0;
	float3	vec = (float3) 0;
	float3	p = (float3) 0, n1 = (float3)0;

	i = 0;
	j = 1;
	while (i < 4096)
	{
		current_position = orig + (*t0) * dir;
		p = current_position - s->center;
		n1 = dot(p, s->vector) * s->vector;
		vec = p - n1;
		q = (float2)(length(vec) - s->radius, length(n1));
		if ((j == 1) && ((length(q) - s->param) < -1e-5f))
			j = -1;
		distance_to_closest = fabs(length(q) - s->param);
		if ((*t0) > 10000.f)
			return (0);
		if (distance_to_closest < 1e-3f)
			return (j);
		(*t0) += distance_to_closest;
		i++;
	}
	return (0);
}

static float	sdf_mandelbulb(float3 pos, float power, int iter, int breakout)
{
	float3 z = pos;
	float dr = 1;
	float r;

	for (int i = 0; i < iter; i++)
	{
		r = fast_length(z);
		if (r > breakout)
			break ;

		float theta = acos(z.z / r) * power;
		float phi = atan2(z.y, z.x) * power;
		float zr = pow(r, power);
		dr = pow(r, power - 1) * power * dr + 1;

		z = zr * (float3)(half_sin(theta) * half_cos(phi), half_sin(phi) * half_sin(theta), half_cos(theta));
		z += pos;
	}
	return ((0.5 * log(r) * r / dr));
}

static int		mandelbulb_intersect(float3 orig, float3 dir, __global t_object *obj, float *dist_to_obj, float *last_dist)
{
	int		max_steps = 500;
	float3	cur_ray_pos = orig;
	float	local_dist_to_obj = 0.f;

	for (int i = 0; i < max_steps; i++)
	{
		local_dist_to_obj = sdf_mandelbulb(cur_ray_pos - obj->center, obj->param, obj->radius, 3);
		if (local_dist_to_obj < F_EPS)
		{
			*last_dist = local_dist_to_obj;
			return (1);
		}
		cur_ray_pos += local_dist_to_obj * dir;
		*dist_to_obj += local_dist_to_obj;
		if (*dist_to_obj > 1000.f)
			return (0);
	}
	return (0);
}

static float ft_dot2(float3 a)
{
	return dot(a, a);
}

static int quad_intersect(float3 orig, float3 dir, __global t_object *s, float *t0)
{
	int		i;
	float3	current_position;
	float	distance_to_closest;
	float3	ba;
	float3	cb;
	float3	dc;
	float3	ad;
	float3	pa;
	float3	pb;
	float3	pc;
	float3	pd;
	float3	nor;
	float	x;
	float3	c;

	ba = s->b - s->a;
	ad = s->a - s->center;
	c = s->center + ba;
	cb = c - s->b;
	dc = s->center - c;

	nor = fast_normalize(cross(ba, ad));
	if (length(nor) < 1e-2)
		return (0);
	i = 0;
	while (i < 4096)
	{
		current_position = orig + (*t0) * dir;
		pa = current_position - s->a;
		pb = current_position - s->b;
		pc = current_position - c;
		pd = current_position - s->center;
		x =(
			(ft_sign(dot(cross(ba, nor), pa)) +
			 ft_sign(dot(cross(cb, nor), pb)) +
			 ft_sign(dot(cross(dc, nor), pc)) +
			 ft_sign(dot(cross(ad, nor), pd)) < 3.f)
			?
			min( min( min(
						  ft_dot2(ba * clamp(dot(ba, pa) / ft_dot2(ba), 0.0f, 1.0f) - pa),
						ft_dot2(cb * clamp(dot(cb, pb) / ft_dot2(cb), 0.0f, 1.0f) - pb)),
					ft_dot2(dc * clamp(dot(dc, pc) / ft_dot2(dc), 0.0f, 1.0f) - pc)),
				ft_dot2(ad * clamp(dot(ad, pd) / ft_dot2(ad), 0.0f, 1.0f) - pd))
			:
		dot(nor, pa) * dot(nor, pa) / ft_dot2(nor));
		distance_to_closest = sqrt(x);
		if ((*t0) > 10000.f)
			return (0);
		if (distance_to_closest < 1e-3f)
			return (1);
		(*t0) += distance_to_closest;
		i++;
	}
	return (0);
}

static int	hyper_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float	a;
	float	b;
	float	c;
	float 	t1;
	float	t2;
	float3	x;
	float	discriminate;

	x = orig - p->center;
	p->vector = fast_normalize(p->vector);
	a = dot(dir, dir) - dot(dir, p->vector) * dot(dir, p->vector);
	b = 2.f * (dot(dir, x) - dot(dir, p->vector) * (dot(x, p->vector) + 2.f * p->param));
	c = dot(x, x) - dot(x, p->vector) * (dot(x, p->vector) + 4.f * p->param);
	discriminate = b * b - 4.f * a * c;
	if (discriminate < -1e-5f)
		return (0);
	if (fabs(discriminate) < 1e-5f)
	{
		*t0 = -b / (a * 2.f);
		return (1);
	}
	t1 = (-b + sqrt(discriminate)) / (2.f * a);
	t2 = (-b - sqrt(discriminate)) / (2.f * a);
	*t0 = min(t1, t2);
	if (*t0 > 1e-3f)
		return (1);
	*t0 = max(t1, t2);
	if (*t0 > 1e-5f)
		return (2);
	return (0);
}

static int	sphere_intersect(float3 orig, float3 dir, __global t_object *s, float *t0)
{
	float3	l;
	float	tca;
	float	d2;
	float	thc;

	l = s->center - orig;
	tca = dot(l, dir);
	d2 = dot(l, l) - tca * tca;
	if (d2 > s->radius * s->radius)
		return (0);
	thc = sqrt(s->radius * s->radius - d2);
	*t0 = tca - thc;
	if (*t0 > 1e-5f)
		return (1);
	*t0 = tca + thc;
	if (*t0 > 1e-5f)
		return (2);
	return (0);
}

static int	plane_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float	a = 0;

	p->vector = fast_normalize(p->vector);
	dir = fast_normalize(dir);
	a = dot(dir, p->vector);
	if (fabs(a) < 1e-6f)
		return (0);
	*t0 = (p->param - dot(orig, p->vector)) / a;
	if ((*t0) < 0.f)
		return (0);
	return (1);
}

static int		cyl_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float	b;
	float3	s;
	float3	q;
	float	c;
	float	discriminate;

	s = dir - p->vector * dot(dir, p->vector);
	q = orig - p->center;
	q = q - p->vector * dot(q, p->vector);
	b = 2 * dot(s, q);
	c = dot(q, q) - p->radius * p->radius;
	discriminate = b * b - 4 * dot(s, s) * c;
	if (fabs(discriminate) < 1e-6f)
		return (0);
	if (discriminate < 0.f)
		return (0);
	*t0 = (-b - sqrt(discriminate)) / (2 * dot(s, s));
	if (*t0 > 0.f)
		return (1);
	*t0 = (-b + sqrt(discriminate)) / (2 * dot(s, s));
	if (*t0 > 0)
		return (2);
	return (0);
}

static int			cone_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float	t1;
	float	t2;
	float	a;
	float	b;
	float	c;
	float3	s;
	float3	q;
	float	discriminate;

	s = dir - p->vector * dot(dir, p->vector);
	q = orig - p->center - p->vector * dot((orig - p->center), p->vector);
	a = cos(p->param) * cos(p->param) * dot(s, s) - sin(p->param) * sin(p->param) * dot(dir, p->vector) * dot(dir, p->vector);
	b = 2 * cos(p->param) * cos(p->param) * dot(s, q) - 2 * sin(p->param) *	sin(p->param) * dot(dir, p->vector) * dot(orig - p->center, p->vector);
	c = cos(p->param) * cos(p->param) * dot(q, q) - sin(p->param) *	sin(p->param) * dot(orig - p->center, p->vector) * dot(orig - p->center, p->vector);
	discriminate = b * b - 4 * a * c;
	if (discriminate < 0.f)
		return (0);
	t1 = (-b + sqrt(discriminate)) / (2.f * a);
	t2 = (-b - sqrt(discriminate)) / (2.f * a);
	if (fabs(t1 - t2) < 1e-6f)
		return (0);
	*t0 = min(t1, t2);
	if (*t0 > 0.f)
	{
		if (acos(fabs(dot(dir, p->vector))) > p->param)
			return (1);
		return (2);
	}
	*t0 = max(t1, t2);
	if (*t0 > 0.f)
	{
		if (acos(fabs(dot(dir, p->vector))) > p->param)
			return (2);
		return (1);
	}
	return (0);
}

int		scene_intersect(float3 orig, float3 dir, __global t_object *obj,
		t_lighting *lighting, int count,  const __global int *texture_w,
		const __global int *texture_h, const __global int *prev_texture_size,
		const __global int *texture)
{
	float 	dist;
	int 	i;
	int		j;
	float	dist_i;
	float2 	uv;
	float3	v;

	dist = MAX_DIST + 1.f;
	i = 0;
	while (i < count)
	{
		if ((*(obj + i)).e_type == o_sphere)
		{
			dist_i = 0.f;
			j = sphere_intersect(orig, dir, (obj + i), &dist_i);
			if (j && dist_i < dist)
			{
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				lighting->n = fast_normalize(lighting->hit - ((*(obj + i)).center));
				if (j == 2)
					lighting->n *= -1.f;
				lighting->mat = (*(obj + i)).mat;
				if ((*(obj + i)).mat.texture_id != -1)
				{
					uv = uv_mapping_for_sphere(lighting);
					normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
				}
			}
		}
		else if ((*(obj + i)).e_type == o_plane)
		{
			dist_i = 0.f;
			j = plane_intersect(orig, dir, (obj + i), &dist_i);
			if (j && dist_i < dist)
			{
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				lighting->n = (*(obj + i)).vector;
				 if (dot(dir, lighting->n) > 0.f)
					lighting->n *= -1.f;
				lighting->mat = (*(obj + i)).mat;
				if ((*(obj + i)).mat.texture_id != -1)
				{
					uv = uv_mapping_for_plane(lighting);
					normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
				}
			}
		}
		else if ((*(obj + i)).e_type == o_cylinder)
		{
			dist_i = 0.f;
			j = cyl_intersect(orig, dir, (obj + i), &dist_i);
			if (j && dist_i < dist)
			{
				float d;
				float3 norm = lighting->n, h = lighting->hit, ori = orig;
				t_material m = lighting->mat;

				d = dist;
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				v = lighting->hit - (*(obj + i)).center;
				lighting->n = (*(obj + i)).vector * dot(v, (*(obj + i)).vector);
				lighting->n = fast_normalize(v - lighting->n);
				if (j == 2)
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
				if ((*(obj + i)).mat.texture_id != -1)
				{
					uv = uv_mapping_for_cylinder(lighting, obj + i);
					normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
				}
				if (length(lighting->hit - (*(obj + i)).center) > (*(obj + i)).param)
				{
					orig = lighting->hit + 1e-3f * dir;
					j = cyl_intersect(orig, dir, (obj + i), &dist_i);
					dist = dist +dist_i;
					if (dist < d)
					{
						lighting->hit = orig + dir * dist_i;
						lighting->mat = (*(obj + i)).mat;
						if ((*(obj + i)).mat.texture_id != -1)
						{
							uv = uv_mapping_for_cylinder(lighting, obj + i);
							normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
						}
						v = lighting->hit - (*(obj + i)).center;
						lighting->n = (*(obj + i)).vector * dot(v, (*(obj + i)).vector);
						lighting->n = -fast_normalize(v - lighting->n);
					}
					if (length(lighting->hit - (*(obj + i)).center) > (*(obj + i)).param || dist > d)
					{
						dist = d;
						lighting->n = norm;
						lighting->hit = h;
						lighting->mat = m;
					}
					orig = ori;
				}
			}
		}
		else if ((*(obj + i)).e_type == o_cone)
		{
			dist_i = 0.f;
			j = cone_intersect(orig, dir, (obj + i), &dist_i);
			if (j && dist_i < dist)
			{
				float d;
				float3 norm = lighting->n, h = lighting->hit, ori = orig;
				t_material m = lighting->mat;

				d = dist;
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				v = fast_normalize(lighting->hit - (*(obj + i)).center);
				lighting->n = (*(obj + i)).vector;
				lighting->n = lighting->n * ft_sign(dot(v, (*(obj + i)).vector));
				lighting->n = fast_normalize(v * dot(v, lighting->n) - lighting->n);
				if (j == 2)
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
				if ((*(obj + i)).mat.texture_id != -1)
				{
					uv = uv_mapping_for_cone(lighting, obj + i);
					normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
				}
				if (length(lighting->hit - (*(obj + i)).center) > (*(obj + i)).radius)
				{
					orig = lighting->hit + 1e-3f * dir;
					j = cone_intersect(orig, dir, (obj + i), &dist_i);
					dist = dist +dist_i;
					if (dist < d)
					{
						lighting->hit = orig + dir * dist_i;
						lighting->mat = (*(obj + i)).mat;
						if ((*(obj + i)).mat.texture_id != -1)
						{
							uv = uv_mapping_for_cone(lighting, obj + i);
							normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
						}
						v = fast_normalize(lighting->hit - (*(obj + i)).center);
						lighting->n = (*(obj + i)).vector;
						lighting->n = lighting->n * ft_sign(dot(v, (*(obj + i)).vector));
						lighting->n = fast_normalize(v * dot(v, lighting->n) - lighting->n);
						if (j == 2)
							lighting->n = -lighting->n;
					}
					if (length(lighting->hit - (*(obj + i)).center) > (*(obj + i)).radius || dist > d)
					{
						dist = d;
						lighting->n = norm;
						lighting->hit = h;
						lighting->mat = m;
					}
					orig = ori;
				}
			}
		}
        else if ((*(obj + i)).e_type == o_hyper)
        {
            dist_i = 0.f;
            j = hyper_intersect(orig, dir, (obj + i), &dist_i);
            if (j && dist_i < dist)
            {
                float d;
                float3 norm = lighting->n, h = lighting->hit;
                t_material m = lighting->mat;
                float3 oori = orig;

                d = dist;
                dist = dist_i;
                lighting->hit = orig + dir * dist_i;
                lighting->mat = (*(obj + i)).mat;
                lighting->n = lighting->hit - (*(obj + i)).center - (*(obj + i)).vector * (dot(lighting->hit - (*(obj + i)).center, (*(obj + i)).vector) + (*(obj + i)).param);
                lighting->n = fast_normalize(lighting->n);
                if (j == 2)
                    lighting->n = -lighting->n;
                if (length(lighting->hit - (*(obj + i)).center) > 100.f)
                {
                    orig = lighting->hit + 1e-3f * dir;
                    dist_i = 0.f;
                    j = hyper_intersect(orig, dir, (obj + i), &dist_i);
                    dist = dist + dist_i;
                    if (dist < d)
                    {
                        lighting->hit = orig + dir * dist_i;
                        lighting->mat = (*(obj + i)).mat;
                        lighting->n = lighting->hit - (*(obj + i)).center - (*(obj + i)).vector * (dot(lighting->hit - (*(obj + i)).center, (*(obj + i)).vector) + (*(obj + i)).param);
                        lighting->n = -fast_normalize(lighting->n);
                    }
                    if (length(lighting->hit - (*(obj + i)).center) > 100.f || dist > d)
                    {
                        dist = d;
                        lighting->n = norm;
                        lighting->hit = h;
                        lighting->mat = m;
                    }
                    orig = oori;
                }
            }
        }
		else if ((*(obj + i)).e_type == o_torus)
		{
			dist_i = 0.f;
			j = torus_intersect(orig, dir, (obj + i), &dist_i);
			if (j && dist_i < dist)
			{
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				lighting->mat = (*(obj + i)).mat;
				float k;
				float3 a;
				k = dot(lighting->hit - (*(obj + i)).center, (*(obj + i)).vector);
				a = lighting->hit - (*(obj + i)).vector * k;
				a = a - (*(obj + i)).center;
				a = fast_normalize(a);
				a = a * (*(obj + i)).radius;
				a = a + (*(obj + i)).center;
				lighting->n = fast_normalize(lighting->hit - a);
				if(fabs(k) < 1e-3f)
				{
					if (length((*(obj + i)).center - lighting->hit) < (*(obj + i)).radius)
						lighting->n = fast_normalize((*(obj + i)).center - lighting->hit);
					else
						lighting->n = -fast_normalize((*(obj + i)).center - lighting->hit);
				}
				if (j == -1)
					lighting->n = -lighting->n;
				if ((*(obj + i)).mat.texture_id != -1)
				{
					uv = uv_mapping_for_torus(lighting, obj + i);
					normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
				}
			}
		}
		else if ((*(obj + i)).e_type == o_sqr)
		{
			dist_i = 0.f;
			j = quad_intersect(orig, dir, (obj + i), &dist_i);
			if (j && dist_i < dist)
			{
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				lighting->n = cross((*(obj + i)).b - (*(obj + i)).a, (*(obj + i)).a - (*(obj + i)).center);
				lighting->n = fast_normalize(lighting->n);
				if (dot(dir, lighting->n) > 0.f)
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
				if ((*(obj + i)).mat.texture_id != -1)
				{
					uv = uv_mapping_for_sqr(lighting, obj + i);
					normalize_coord_for_texture((obj + i), uv, &(lighting->mat.diffuse_color), texture, texture_w, texture_h, prev_texture_size);
				}
			}
		}
		else if ((*(obj + i)).e_type == o_mandelbulb)
		{
			dist_i = 0.f;
			float last_dist;
			j = mandelbulb_intersect(orig, dir, (obj + i), &dist_i, &last_dist);
			if (j && dist_i < dist)
			{
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				lighting->mat = (*(obj + i)).mat;
				float3 pos = lighting->hit;
				lighting->n = normalize((float3){
					sdf_mandelbulb((float3){pos.x + F_EPS, pos.y, pos.z} - (*(obj + i)).center, (*(obj + i)).param, (*(obj + i)).radius, 3),
					sdf_mandelbulb((float3){pos.x, pos.y + F_EPS, pos.z} - (*(obj + i)).center, (*(obj + i)).param, (*(obj + i)).radius, 3),
					sdf_mandelbulb((float3){pos.x, pos.y, pos.z + F_EPS} - (*(obj + i)).center, (*(obj + i)).param, (*(obj + i)).radius, 3)} -
							(float3){last_dist, last_dist, last_dist});
			}
		}
	i++;
	}
	return (dist < MAX_DIST);
}
