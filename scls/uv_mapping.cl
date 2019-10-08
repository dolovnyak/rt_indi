#include "./includes/kernel.h"
#include "./includes/rt.h"

float3 vec_change(t_lighting *lighting, __global t_object *obj)
{
    float3 n = obj->vector;
    float3 vec = lighting->hit - obj->center;
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

float2			uv_mapping_for_sphere(t_lighting *lighting)
{
	float3	vec;
	float 	v;
	float 	u;

	vec = lighting->n;
	u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
	v = 0.5f - (asin(vec.z) / M_PI_F);
	return ((float2){u, v});
}

float2			uv_mapping_for_cylinder(t_lighting *lighting, __global t_object *obj)
{
	float3	vec;
	float 	v;
	float 	u;

	vec = vec_change(lighting, obj);

	u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
    v = 0.5f - (modf(vec.z / obj->radius * 250 / 1024, &v) / 2);
	return ((float2){u, v});
}

float2			uv_mapping_for_torus(t_lighting *lighting, __global t_object *obj)
{
	float3	vec = (lighting->hit - obj->center);
    vec = vec_change(lighting, obj);
	float	u = 0.5f + (atan2(vec.x, vec.y) / (2.f * M_PI_F));
	float 	v = 0.5f - asin(vec.z / obj->param) / M_PI_F;
	return ((float2){u, v});
}

float2			uv_mapping_for_plane(t_lighting *lighting)
{
	float3 vec;
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

float2 			uv_mapping_for_cone(t_lighting *lighting, __global t_object *obj)
{
	float3 vec = lighting->hit;

	vec = vec_change(lighting, obj);


	float u;
	float p = (vec.x / vec.z) / tan(obj->param);

	if (vec.y > 0)
		u = acos(p);
	else
		u = 2 * M_PI - acos(p);
	u /= (2 * M_PI);
	float v = 0.5 - modf(vec.z * 100 / 1024, &v) / 2;
	return ((float2) {u, v});
}
