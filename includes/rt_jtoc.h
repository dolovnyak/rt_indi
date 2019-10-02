
#ifndef RT_JTOC_H
# define RT_JTOC_H

extern char		*g_err_str;

int			rt_jtoc_sdl_log_error(const char *p, const int id);
int			rt_jtoc_mouse_setup(t_rt *rt, const char *json);
int			rt_jtoc_textures_setup(t_rt *rt, const char *json);
int			rt_jtoc_scene_setup(t_rt *rt, const char *json);
int			rt_jtoc_get_float2(cl_float2 *vec, t_jnode *n);
int			rt_jtoc_get_float3(cl_float3 *vec, t_jnode *n);
int			rt_jtoc_get_float4(cl_float4 *vec, t_jnode *n);
int			rt_jtoc_get_camera(t_cam *camera, t_jnode *n);
int			rt_jtoc_get_objects_num_in_arr(unsigned int *objects_num,
											t_jnode *n);
int			rt_jtoc_get_objects(t_rt *rt, t_jnode *n,
									t_obj_texture *texture);
int			rt_jtoc_get_object_texture(t_object *obj, t_obj_texture *texture,
										t_jnode *n);
int			rt_jtoc_get_object_type(t_object *obj, t_jnode *n);
int			rt_jtoc_get_phong_param(t_object *obj, t_jnode *n);
int			rt_jtoc_get_effects(t_rt *rt, t_jnode *n);
int			rt_jtoc_get_lights(t_rt *rt, t_jnode *n);
int			rt_jtoc_get_sphere(t_object *obj, t_jnode *n);
int			rt_jtoc_get_torus(t_object *obj, t_jnode *n);
int			rt_jtoc_get_plane(t_object *obj, t_jnode *n);
int			rt_jtoc_get_cone(t_object *obj, t_jnode *n);
int			rt_jtoc_get_cylinder(t_object *obj, t_jnode *n);
int			rt_jtoc_get_hyper(t_object *obj, t_jnode *n);
int			rt_jtoc_get_sqr(t_object *obj, t_jnode *n);
#endif
