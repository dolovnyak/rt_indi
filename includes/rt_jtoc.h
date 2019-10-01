//
// Created by Racquel Keli on 29/09/2019.
//

#ifndef INC_1_92_RT_JTOC_H
#define INC_1_92_RT_JTOC_H

int				rt_jtoc_sdl_log_error(const char *p, const int id);
int				rt_jtoc_textures_setup(t_rt *rt, const char *json);
int				rt_jtoc_scene_setup(t_rt *rt, const char *json);
int				rt_jtoc_get_float2(cl_float2 *vec, t_jnode *n);
int				rt_jtoc_get_float3(cl_float3 *vec, t_jnode *n);
int				rt_jtoc_get_float4(cl_float4 *vec, t_jnode *n);
int				rt_jtoc_get_camera(t_cam *camera, t_jnode *n);
int				rt_jtoc_get_objects_num_in_arr(int *objects_num,
												  t_jnode *n);
int				rt_jtoc_get_objects(t_rt *rt, t_jnode *n,
									   t_obj_texture *texture);
int				rt_jtoc_get_effects(t_rt *rt, t_jnode *n);
int				rt_jtoc_get_sphere(t_object *obj, t_jnode *n);
int				rt_jtoc_get_torus(t_object *obj, t_jnode *n);
int				rt_jtoc_get_plane(t_object *obj, t_jnode *n);
int				rt_jtoc_get_cone(t_object *obj, t_jnode *n);
int				rt_jtoc_get_cylinder(t_object *obj, t_jnode *n);

#endif //INC_1_92_RT_JTOC_H
