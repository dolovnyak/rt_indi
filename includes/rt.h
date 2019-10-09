/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 13:57:47 by broggo            #+#    #+#             */
/*   Updated: 2019/09/30 02:12:51 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# ifndef OPENCL___
#  include "mlx.h"
#  include "libft.h"
#  include "libjtoc.h"
#  include "libcl.h"
#  include "get_next_line.h"
#  include <math.h>
#  include <stdlib.h>
#  include <fcntl.h>
#  include <stdio.h>
#  include <time.h>
#  include <OpenCL/cl.h>
#  include <sys/time.h>
# endif

# define WIDTH		1280
# define HEIGHT		1024

# ifndef OPENCL___
#  define SPEED1 0.5f
#  define SPEED2 (float)(M_PI / 90)
#  define SPEED3 1.f
#  define SPEED4 1.f
#  define CHANNEL_NUM 3
# else
#  define MAX_DIST	10000.f
#  define SIGMA		3
#  define F_EPS		0.001
# endif

# define PHONG		(1 << 0)
# define PATH_TRACE	(1 << 1)
# define GRAY (1 << 0)
# define THREE_D (1 << 1)
# define GAYSS_BLURE (1 << 2)
# define DITHERING (1 << 3)

enum					e_object_type
{
	o_sphere = 1,
	o_plane,
	o_cylinder,
	o_cone,
	o_hyper,
	o_torus,
	o_sqr,
	o_mandelbulb,
};

typedef struct			s_light
{
# ifndef OPENCL___

	cl_float3			center;
	cl_float			intens;
# else

	float3				center;
	float				intens;

# endif

}						t_light;

typedef struct			s_material
{
# ifndef OPENCL___

	cl_float3			diffuse_color;
	cl_float2			al;
	float				sp_ex;
	float				refraction;
	float				reflection;
	cl_float3			emission;
	int					texture_id;
# else

	float3				diffuse_color;
	float2				al;
	float				sp_ex;
	float				refraction;
	float				reflection;
	float3				emission;
	int					texture_id;
# endif

}						t_material;

typedef struct			s_object
{
# ifndef OPENCL___

	cl_float3			center;
	cl_float3			vector;
	float				radius;
	float				param;
	cl_float3			a;
	cl_float3			b;
	enum e_object_type	e_type;
	t_material			mat;
# else
	float3				center;
	float3				vector;
	float				radius;
	float				param;
	float3				a;
	float3				b;
	enum e_object_type	e_type;
	t_material			mat;
# endif

}						t_object;

typedef struct			s_screen
{
# ifndef OPENCL___

	cl_float3			v1;
	cl_float3			v2;
	cl_float3			center;
	cl_float			brightness;
	cl_int				fsaa_n;
	cl_int				effects;
	cl_int				params;
	cl_int				samples;
# else

	float3				v1;
	float3				v2;
	float3				center;
	float				brightness;
	int					fsaa_n;
	int					effects;
	int					params;
	int					samples;
# endif

}						t_screen;

typedef struct			s_cam
{
# ifndef OPENCL___

	cl_float3			center;
	cl_float			alpha;
	cl_float			betta;
# else

	float3				center;
	float				alpha;
	float				betta;
# endif

}						t_cam;

typedef struct			s_counter
{
# ifndef OPENCL___

	cl_int				l;
	cl_int				all_obj;
# else

	int					l;
	int					all_obj;
# endif

}						t_counter;

# ifndef OPENCL___

typedef struct			s_mouse
{
	int					r;
	int					l;
	float				alpha;
	float				betta;
	int					x;
	int					y;
	cl_float3			center;
}						t_mouse;

typedef struct			s_img
{
	void				*img_ptr;
	int					*data;
	int					size_l;
	int					bpp;
	int					endian;
}						t_img;

typedef struct			s_obj_texture
{
	char				**textures_path;
	unsigned int		textures_count;
	int					w;
	int					h;
	int					*texture;
	int					bpp;
	int					texture_w[100];
	int					texture_h[100];
	int					prev_texture_size[100];
	size_t				texture_size;
}						t_obj_texture;

typedef struct			s_gpu_mem
{
	cl_mem				cl_texture;
	cl_mem				cl_texture_w;
	cl_mem				cl_texture_h;
	cl_mem				cl_prev_texture_size;
	cl_mem				cl_img_buffer;
	cl_mem				cl_aux_buffer;
	cl_mem				cl_light_buffer;
	cl_mem				cl_obj_buffer;
	cl_mem				cl_counter_buffer;
}						t_gpu_mem;

typedef struct 			s_fps
{
	struct	timeval		stop;
	struct	timeval		start;
	float 				fps;
}						t_fps;

typedef struct			s_rt
{
	cl_float			ambient;
	void				*mlx_ptr;
	void				*win;
	t_img				img;
	t_cl				*cl;
	t_screen			screen;
	t_cam				cam;
	t_counter			counter;
	t_object			*obj;
	t_light				*light;
	t_mouse				mouse;
	t_obj_texture		*texture;
	t_gpu_mem			*gpu_mem;
	t_fps				fps;
}						t_rt;

int						check_key(int keycode, t_rt *rt);
int						ft_esc(t_rt *rt);
int						ft_aa(int keycode, t_rt *rt);
int						ft_effects(int keycode, t_rt *rt);
int						ft_move_x(int keycode, t_rt *rt);
int						ft_move_y(int keycode, t_rt *rt);
int						ft_move_alpha(int keycode, t_rt *rt);
int						ft_move_betta(int keycode, t_rt *rt);
int						ft_move_z(int keycode, t_rt *rt);
int						mouse_press(int button, int x, int y, t_rt *rt);
int						mouse_move(int x, int y, t_rt *rt);
int						mouse_release(int button, int x, int y, t_rt *rt);
int						ft_move_mouse(t_rt *rt, int button, int x, int y);
int						ft_mouse_alpha_betta(t_rt *rt, int x, int y);
int						ft_mouse_x_y(t_rt *rt, int x, int y);
cl_float3				cl_sum(cl_float3 v1, cl_float3 v2);
cl_float3				cl_minus(cl_float3 v1, cl_float3 v2);
float					cl_length(cl_float3 v);
cl_float3				cl_mult_n(cl_float3 v1, float n);
void					cl_to_norm(cl_float3 *v);
cl_float3				cl_normalize(cl_float3 v1);
cl_float3				cl_cross(cl_float3 v1, cl_float3 v2);
float					cl_dot(cl_float3 v1, cl_float3 v2);
float					cl_angle(cl_float3 v1, cl_float3 v2);
void					draw_picture(t_rt *rt);
int						cl_worker(t_rt *rt);
cl_device_id			create_device();
cl_program				build_program(cl_context ctx, cl_device_id dev,
											const char *filename);
int						read_map(char *name, t_rt *rt);
cl_float3				spherical(float phi, float tetta);
cl_float3				spher_norm(cl_float3 v);
void					calc_screen(t_screen *screen, t_cam *cam);
void					get_textures(t_rt *rt, char **texture_file,
												int number_of_texture);
void					find_textures_size(t_rt *rt, char **texture_file,
												int number_of_texture);
void					fill_gpu_mem(t_rt *rt);
void					release_gpu_mem(t_rt *rt);
int						gauss_blur_gpu_init(t_rt *rt, cl_kernel *kernel,
											cl_mem screen_buffer);

# endif

#endif
