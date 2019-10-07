
#include "./includes/kernel.h"
#include "./includes/rt.h"

#  define RED(c)			(((int)c >> 16) & 0xFF)
#  define GREEN(c)			(((int)c >> 8) & 0xFF)
#  define BLUE(c)			((int)c & 0xFF)


static int		get_light(int start, int end, float percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

static int		get_color(float3 v, int type)
{
	int     red;
	int     green;
	int     blue;
	int 	start;
	int		end;

	float e;
	e  = (max(max(v.x, v.y), v.z));
	if (e > 1)
		v *= 1.f / e;
	if (type & GRAY)
	{
		float3	c_linear;
		float	y_linear;

		c_linear.x = v.x > 0.04045f ? pow((v.x + 0.055f) / 1.055f, 2.4f) : v.x / 12.92f;
		c_linear.y = v.y > 0.04045f ? pow((v.y + 0.055f) / 1.055f, 2.4f) : v.y / 12.92f;
		c_linear.z = v.z > 0.04045f ? pow((v.z + 0.055f) / 1.055f, 2.4f) : v.z / 12.92f;

		y_linear = 0.2126f * c_linear.x + 0.7152f * c_linear.y + 0.0722f * c_linear.z;
		v.x = y_linear > 0.0031308f ? 1.055f * pow(y_linear, 1.f / 2.4f) - 0.055f  : 12.92f * y_linear;
		v.y = v.x;
		v.z = v.x;
	}
	start = 0;
	end = 0xFFFFFF;
	red = get_light((start >> 16) & 0xFF, (end >> 16) & 0xFF, v.x);
	green = get_light((start >> 8) & 0xFF, (end >> 8) & 0xFF, v.y);
	blue = get_light(start & 0xFF, end & 0xFF, v.z);

	return ((red << 16) | (green << 8) | blue);
}

static void	normalize_coord_for_texture(t_object obj, float2 uv, float3 *color,
									__global int *texture, __global int *texture_w, __global int *texture_h,
									__global int *prev_texture_size)
{
	int coord;
	int coord_x;
	int coord_y;
	int 	red;
	int 	green;
	int     blue;
	int 	start;
	int		end;

	coord_x = (int)((uv.x * texture_w[obj.mat.texture_id]));
	coord_y = (int)((uv.y * texture_h[obj.mat.texture_id]));
	coord_y *= (texture_w[obj.mat.texture_id]);
	coord = coord_x + coord_y;
	coord += prev_texture_size[obj.mat.texture_id];

	start = 0;
	end = 0xFFFFFF;
	color->x = (RED(texture[coord]));
	color->y = (GREEN(texture[coord]));
	color->z = (BLUE(texture[coord]));
	color->x /= 255;
	color->y /= 255;
	color->z /= 255;
}

static float3 vec_change(t_lighting *lighting, t_object obj)
{
    float3 n = obj.vector;
    float3 vec = lighting->hit - obj.center;
    float cos_x, cos_z;
    float3 new_vec, new_n;
    float alpha_x, alpha_z;

    if (length((float2)(n.x, n.y)) < 1e-5f)
        return vec;
    else
    {
        cos_x = n.x / length((float2)(n.x, n.y));
        if (n.y > 0)
            alpha_x = acos(cos_x);
        else
            alpha_x = -acos(cos_x);
    }

    new_vec.x = dot((float3)(cos(alpha_x), sin(alpha_x), 0), vec);
    new_vec.y = dot((float3)(-sin(alpha_x), cos(alpha_x), 0), vec);
    new_vec.z = dot((float3)(0, 0, 1), vec);

    new_n.x = dot((float3)(cos(alpha_x), sin(alpha_x), 0), n);
    new_n.y = dot((float3)(-sin(alpha_x), cos(alpha_x), 0), n);
    new_n.z = dot((float3)(0, 0, 1), n);


    cos_z = new_n.z / length(new_n);
    if (new_n.x > 0)
        alpha_z = acos(cos_z);
    else
        alpha_z = -acos(cos_z);

    vec.x = dot((float3)(cos(alpha_z), 0, -sin(alpha_z)), new_vec);
    vec.y = dot((float3)(0, 1, 0), new_vec);
    vec.z = dot((float3)(sin(alpha_z), 0, cos(alpha_z)), new_vec);

    return vec;
}

static float2			uv_mapping_for_sphere(t_lighting *lighting, t_object obj)
{
	float3	vec;
	float 	v;
	float 	u;

	vec = lighting->n;
	u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
	v = 0.5f - (asin(vec.z) / M_PI_F);
	return ((float2){u, v});
}

static float2			uv_mapping_for_cylinder(t_lighting *lighting, t_object obj)
{
	float3	vec;
	float 	v;
	float 	u;

	vec = vec_change(lighting, obj);

	u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
    v = 0.5f - (modf(vec.z / obj.radius * 250 / 1024, &v) / 2);
	return ((float2){u, v});
}

static float2			uv_mapping_for_torus(t_lighting *lighting, t_object obj)
{
	float3	vec = (lighting->hit - obj.center);
    vec = vec_change(lighting, obj);
	float	u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
	float 	v = 0.5f - asin(vec.z / obj.param) / M_PI_F;
	return ((float2){u, v});
}

static float2			uv_mapping_for_plane(t_lighting *lighting, t_object obj)
{
	float3 vec;
	float3 n;
	float3 normvec;
	float3 crossvec;
	float v;
	float u;

    vec = lighting->hit;

	if (lighting->n.x != 0.0f || lighting->n.y != 0.0f)
		normvec = normalize((float3) {lighting->n.y, -lighting->n.x, 0.0f});
	else
		normvec = (float3) {0.0f, 1.0f, 0.0f};

	crossvec = cross(lighting->n, normvec);
	u = 0.5 + fmod(dot(normvec, vec), 16.0f) / 32; //  '16.0f) / 32' - tiling of textute
	v = 0.5 + fmod(dot(crossvec, vec), 16.0f) / 32;
	return ((float2){u, v});
}

static float2 			uv_mapping_for_cone(t_lighting *lighting, t_object obj)
{
    float3 vec = lighting->hit;

    vec = vec_change(lighting, obj);




    float u;
    float p = (vec.x / vec.z) / tan(obj.param);

    if (vec.y > 0)
        u = acos(p);
    else
        u = 2 * M_PI - acos(p);
    u /= (2 * M_PI);
    float v = 0.5 - modf(vec.z * 100 / 1024, &v) / 2;
    return ((float2){u, v});
}

static int		choose_texture_for_object(const __global t_object *obj,  __global int *texture,
		float3 *color, __global int *texture_w, __global int *texture_h,
		__global int *prev_texture_size, t_lighting *lighting, int i)
{
	float2	uv;
	int 	found_texture_for_obj;
	t_object tmp_obj = *(obj + i);

	found_texture_for_obj = 1;
	return (found_texture_for_obj);
	if (tmp_obj.mat.texture_id == -1)
		return (found_texture_for_obj);
	uv = (float2){-1.f, -1.f};
	if (tmp_obj.e_type == o_sphere)
		uv = uv_mapping_for_sphere(lighting, tmp_obj);
	else if (tmp_obj.e_type == o_cylinder)
		uv = uv_mapping_for_cylinder(lighting, tmp_obj);
	else if (tmp_obj.e_type == o_torus)
		uv = uv_mapping_for_torus(lighting, tmp_obj);
	else if (tmp_obj.e_type == o_plane)
		uv = uv_mapping_for_plane(lighting, tmp_obj);
	else if (tmp_obj.e_type == o_cone)
		uv = uv_mapping_for_cone(lighting, tmp_obj);
	if (uv.x != -1.f && uv.y != -1.f)
	{
		found_texture_for_obj = 0;
		normalize_coord_for_texture(tmp_obj, uv, color, texture, texture_w, texture_h, prev_texture_size);
		return (found_texture_for_obj);
	}
	return (found_texture_for_obj);
}

static int	ft_sign(float a)
{
	if (a > 1e-3)
		return 1;
	if (a < -1e-3)
		return -1;
	return 0;
}

static int torus_intersect(float3 orig, float3 dir, __global t_object *s, float *t0)
{
	int		i;
	float3	current_position = (float3)0;
	float	distance_to_closest = 0.f;
	float2	q = (float2)0;
	float3	vec = (float3)0;
	float3	p = (float3)0, n1 = (float3)0;

	i = 0;
	int j = 1;
	//float3 gt = clamp(current_position, orig, dir);
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
	return 0;
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
//		distance_to_obj = sdf_mandelbulb(local_pos, obj->params.mandelbulb.power,
//				obj->params.mandelbulb.iteration, obj->params.mandelbulb.breakout);
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
		return 0;
	i = 0;
	while (i < 4096)
	{
		current_position = orig + (*t0) * dir;
		pa = current_position - s->a;
		pb = current_position - s->b;
		pc = current_position - c;
		pd = current_position - s->center;
		x =(
			(ft_sign(dot(cross(ba,nor),pa)) +
			 ft_sign(dot(cross(cb,nor),pb)) +
			 ft_sign(dot(cross(dc,nor),pc)) +
			 ft_sign(dot(cross(ad,nor),pd)) < 3.0f)
			?
			min( min( min(
						  ft_dot2(ba*clamp(dot(ba,pa)/ft_dot2(ba),0.0f,1.0f)-pa),
						ft_dot2(cb*clamp(dot(cb,pb)/ft_dot2(cb),0.0f,1.0f)-pb) ),
					ft_dot2(dc*clamp(dot(dc,pc)/ft_dot2(dc),0.0f,1.0f)-pc) ),
				ft_dot2(ad*clamp(dot(ad,pd)/ft_dot2(ad),0.0f,1.0f)-pd) )
			:
		dot(nor,pa)*dot(nor,pa)/ft_dot2(nor) );
		distance_to_closest = sqrt(x);
		if ((*t0) > 10000.f)
			return (0);
		if (distance_to_closest < 1e-3f)
			return (1);
		(*t0) += distance_to_closest;
		i++;
	}
	return 0;
}

static int	hyper_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float a;
	float b;
	float c;
	float t1;
	float t2;
	float3 x;

	x = orig - p->center;
	p->vector = fast_normalize(p->vector);
	a = dot(dir, dir) - dot(dir, p->vector) * dot(dir, p->vector);
	b = 2.f * (dot(dir, x) - dot(dir, p->vector) * (dot(x, p->vector) + 2.f * p->param));
	c = dot(x, x) - dot(x, p->vector) * (dot(x, p->vector) + 4.f * p->param);
	if (b * b - 4.f * a * c < -1e-5)
		return 0;
	if (fabs(b * b - 4.f * a * c) < 1e-5)
	{
		*t0 = -b / (a * 2.f);
		return 1;
	}
	t1 = (-b + sqrt(b * b - 4.f * a * c)) / (2.f * a);
	t2 = (-b - sqrt(b * b - 4.f * a * c)) / (2.f * a);
	*t0 = min(t1, t2);
	if (*t0 > 1e-3)
	{
		return 1;
	}
	*t0 = max(t1, t2);
	if (*t0 > 1e-5)
	{
		return 2;
	}
	return 0;
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
	if (fabs(a) < 1e-6)
		return (0);
	*t0 = (p->param - dot(orig, p->vector)) / a;
	if ((*t0) < 0)
		return (0);
	return (1);
}

static int		cyl_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float b;
	float3	s;
	float3	q;
	float c;
	float d;

	s = dir - p->vector * dot(dir, p->vector);
	q = orig - p->center;
	q = q - p->vector * dot(q, p->vector);
	b = 2 * dot(s, q);
	c = dot(q, q) - p->radius * p->radius;
	d = b * b - 4 * dot(s, s) * c;
	if (fabs(d) < 1e-6)
		return (0);
	if (d < 0)
		return (0);
	*t0 = ((-1) * b - sqrt(d)) / (2 * dot(s, s));
	if (*t0 > 0)
		return (1);
	*t0 = (-b + sqrt(d)) / (2 * dot(s, s));
	if (*t0 > 0)
		return (2);
	return (0);
}

static int			cone_intersect(float3 orig, float3 dir, __global t_object *p, float *t0)
{
	float t1;
	float t2;
	float a;
	float b;
	float c;
	float3	s;
	float3	q;

	s = dir - p->vector * dot(dir, p->vector);
	q = orig - p->center - p->vector * dot((orig - p->center), p->vector);
	a = cos(p->param) * cos(p->param) * dot(s, s) -
	sin(p->param) * sin(p->param) * dot(dir, p->vector) * dot(dir, p->vector);
	b = 2 * cos(p->param) * cos(p->param) * dot(s, q) - 2 * sin(p->param) *	sin(p->param) * dot(dir, p->vector) * dot(orig - p->center, p->vector);
	c = cos(p->param) * cos(p->param) * dot(q, q) - sin(p->param) *	sin(p->param) * dot(orig - p->center, p->vector) * dot(orig - p->center, p->vector);
	if (b * b - 4 * a * c < 0)
		return (0);
	t1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	if (fabs(t1 - t2) < 1e-6)
		return (0);
	*t0 = min(t1, t2);
	if (*t0 > 0)
	{
		if (acos(fabs(dot(dir, p->vector))) > p->param)
			return (1);
		return (2);
	}
	*t0 = max(t1, t2);
	if (*t0 > 0)
	{
		if (acos(fabs(dot(dir, p->vector))) > p->param)
			return (2);
		return (1);
	}
	return (0);
}

static int		scene_intersect(float3 orig, float3 dir, const __global t_object *obj,
		t_lighting *lighting, int count, __global int *texture_w,
		__global int *texture_h, __global int *prev_texture_size,
		__global int *texture)
{
	float 	dist;
	int 	i;
	int		j;
	float	dist_i;
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
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
				float3 col1 = (float3){1, 0, 0};
				if (!(choose_texture_for_object(obj, texture, &col1,
						texture_w, texture_h, prev_texture_size, lighting, i)))
					lighting->mat.diffuse_color = col1;
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
				 if (dot(dir, lighting->n) > 0)
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
				float3 col1 = (float3){1, 0, 0};
				if (!(choose_texture_for_object(obj, texture, &col1,
					texture_w, texture_h, prev_texture_size, lighting, i)))
					lighting->mat.diffuse_color = col1;
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
				float3 col1 = (float3){1, 0, 0};
				if (!(choose_texture_for_object(obj, texture, &col1,
												texture_w, texture_h, prev_texture_size, lighting, i)))
					lighting->mat.diffuse_color = col1;
				if (length(lighting->hit - (*(obj + i)).center) > 50.f)
				{
					orig = lighting->hit + 1e-3f * dir;
					j = cyl_intersect(orig, dir, (obj + i), &dist_i);
					dist = dist +dist_i;
					if (dist < d)
					{
						lighting->hit = orig + dir * dist_i;
						lighting->mat = (*(obj + i)).mat;
						float3 col1 = (float3){1, 0, 0};
						if (!(choose_texture_for_object(obj, texture, &col1,
														texture_w, texture_h, prev_texture_size, lighting, i)))
							lighting->mat.diffuse_color = col1;
						v = lighting->hit - (*(obj + i)).center;
						lighting->n = (*(obj + i)).vector * dot(v, (*(obj + i)).vector);
						lighting->n = -fast_normalize(v - lighting->n);
					}
					if (length(lighting->hit - (*(obj + i)).center) > 50.f || dist > d)
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
				dist = dist_i;
				lighting->hit = orig + dir * dist_i;
				v = fast_normalize(lighting->hit - (*(obj + i)).center);
				lighting->n = (*(obj + i)).vector;
				lighting->n = lighting->n * ft_sign(dot(v, (*(obj + i)).vector));
				lighting->n = fast_normalize(v * dot(v, lighting->n) - lighting->n);
				if (j == 2)
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
				float3 col1 = (float3){1, 0, 0};
				if (!(choose_texture_for_object(obj, texture, &col1,
												texture_w, texture_h, prev_texture_size, lighting, i)))
					lighting->mat.diffuse_color = col1;
				if (length(lighting->hit - (*(obj + i)).center) > 50.f)
				{
					orig = lighting->hit + 1e-3f * dir;
					j = cone_intersect(orig, dir, (obj + i), &dist_i);
					dist = dist +dist_i;
					if (dist < d)
					{
						lighting->hit = orig + dir * dist_i;
						lighting->mat = (*(obj + i)).mat;
						float3 col1 = (float3){1, 0, 0};
						if (!(choose_texture_for_object(obj, texture, &col1,
														texture_w, texture_h, prev_texture_size, lighting, i)))
							lighting->mat.diffuse_color = col1;
						v = fast_normalize(lighting->hit - (*(obj + i)).center);
						lighting->n = (*(obj + i)).vector;
						lighting->n = lighting->n * ft_sign(dot(v, (*(obj + i)).vector));
						lighting->n = fast_normalize(v * dot(v, lighting->n) - lighting->n);
						if (j == 2)
							lighting->n = -lighting->n;
					}
					if (length(lighting->hit - (*(obj + i)).center) > 50.f || dist > d)
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
				float m;
				k = dot(lighting->hit - (*(obj + i)).center, (*(obj + i)).vector);
				a = lighting->hit - (*(obj + i)).vector * k;
				a = a - (*(obj + i)).center;
				a = fast_normalize(a);
				a = a * (*(obj + i)).radius;
				a = a + (*(obj + i)).center;
				lighting->n = fast_normalize(lighting->hit - a);
				if(fabs(k) < 1e-3)
				{
					if (length((*(obj + i)).center - lighting->hit) < (*(obj + i)).radius)
					{
						lighting->n = fast_normalize((*(obj + i)).center - lighting->hit);
					}
					else
					{
						lighting->n = -fast_normalize((*(obj + i)).center - lighting->hit);

					}
				}
				if (j == -1)
					lighting->n = -lighting->n;
				float3 col1 = (float3){1, 0, 0};
				if (!(choose_texture_for_object(obj, texture, &col1,
						texture_w, texture_h, prev_texture_size, lighting, i)))
					lighting->mat.diffuse_color = col1;
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
				if (dot(dir, lighting->n) > 0)
					lighting->n *= -1;
				lighting->mat = (*(obj + i)).mat;
			}
		}
		else if ((*(obj + i)).e_type == o_mandelbulb)
		{
			dist_i = 0;
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

static float3	light_shadow(float3 dir, const __global t_object *obj,
		const __global t_light *l, t_lighting *lighting,
		const __global t_counter *counter, __global int *texture_w,
		__global int *texture_h, __global int *prev_texture_size,
		__global int *texture, float ambient)
{
	float		light_dist = 0;
	float3		light_dir = (float3) 0;
	float3		shadow_orig = (float3) 0;
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

static float3 trace(float3 orig, float3 dir, const __global t_object *obj, int count,
			const int* seed0, const int* seed1,
			__global int *texture_w, __global int *texture_h,
			__global int *prev_texture_size, __global int *texture, float brightness)
{
	float3 path_color = (float3)(0.0f, 0.0f, 0.0f);
	float3 mask = (float3)(1.0f, 1.0f, 1.0f);
	int randSeed0 = seed0;
	int randSeed1 = seed1;
	t_lighting lighting;
	lighting.n = (float3) 0;
	lighting.hit = (float3) 0;
	float3 path_orig = orig, path_dir = dir;
	float mirr = 0.f;

	int ind = 0;
	for (int bounces = 0; bounces < 8; bounces++)
	{
		if(!scene_intersect(path_orig, path_dir, obj, &lighting, count,
		texture_w, texture_h, prev_texture_size, texture))
		{
			path_color += mask * 0.01f;
			break;
		}
		float rand1 = get_random(randSeed0, randSeed1) * 2.0f * M_PI_F;
		float rand2 = get_random(randSeed1, randSeed0);
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

static float	reverse(int n)
{
	if (n != 0)
		return (1.0f / n);
	return (0);
}

__kernel void	rt(
					__global int				*data,
					const __global t_cam		*cam,
					const __global t_screen		*screen,
					const __global t_counter	*counter,
					const __global t_light		*l,
					const __global t_object		*obj,
					int2 rands, float ambient,
					__global int *texture,	__global int *texture_w,
					__global int *texture_h,
					__global int *prev_texture_size)
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

	if (screen->params & PATH_TRACE)
	{
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
	}
	else if (screen->params & PHONG)
	{
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
	}

	color = color / ((fsaa + 1) * (fsaa + 1));

	data[index] = get_color(color, screen->effects);
}
