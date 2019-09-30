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

# include "mlx.h"
# include "libft.h"
# include "libjtoc.h"
# include "libcl.h"
# include "get_next_line.h"
# include <math.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <time.h>
# include <OpenCL/cl.h>

# define WIDTH		1280
# define HEIGHT		1024

#define	SPEED1 0.5f
#define	SPEED2 (float)(M_PI / 90)
#define	SPEED3 1.f
#define	SPEED4 1.f

#define PROGRAM_FILE "./scls/rt.cl"
#define KERNEL_FUNC_RT "rt"
#define KERNEL_FUNC_PP "post_processing"
#define KERNEL_FUNC_GB_X "gauss_blur_x"
#define KERNEL_FUNC_GB_Y "gauss_blur_y"

#define CHANNEL_NUM 3

enum					e_object_type
{
	o_sphere = 1,
	o_torus,
	o_cylinder,
	o_cone,
	o_plane,
};

typedef struct		s_light
{
	cl_float3		center;
	cl_float		intens;
}					t_light;

typedef struct		s_material
{
	cl_float3		diffuse_color;
	cl_float2		al;
	float			sp_ex;
	cl_float3		emission;
    int             texture_id;
}					t_material;

typedef struct		s_object
{
	int					type;
	cl_float3			center;
	cl_float3			vector;
	float				radius;
	float				param;
	cl_float3			a;
	cl_float3			b;
	cl_float3			c;
	cl_float3			d;
	enum e_object_type	e_type;
	t_material			mat;
}					t_object;

typedef struct		s_img
{
	void			*img_ptr;
	int				*data;
	int				size_l;
	int				bpp;
	int				endian;
}					t_img;

typedef struct			s_cl_arg
{
	cl_device_id		device;
	cl_context			context;
	cl_program			program;
	cl_command_queue	queue;
}						t_cl_arg;

typedef struct		s_screen
{
	cl_float3		v1;
	cl_float3		v2;
	cl_float3		center;
	int				fsaa_n;
	cl_int8			effects;
}					t_screen;

typedef struct		s_cam
{
	cl_float3		center;
	cl_float		alpha;
	cl_float		betta;
}					t_cam;

typedef struct		s_counter
{
	int				l;
	int				all_obj;
}					t_counter;

typedef struct		s_mouse
{
	int				r;
	int				l;
	float			alpha;
	float			betta;
	int				x;
	int				y;
	cl_float3		center;
}					t_mouse;

typedef struct 			s_obj_texture
{
    char				**textures_path;
    unsigned int		textures_count;
    int 				w;
    int 				h;
    int 				*texture;
    int 				bpp;
    int					texture_w[100];
    int					texture_h[100];
    int					prev_texture_size[100];
    size_t				texture_size;
}						t_obj_texture;

typedef struct 		s_gpu_mem
{
	cl_mem			cl_texture;
	cl_mem			cl_texture_w;
	cl_mem			cl_texture_h;
	cl_mem			cl_prev_texture_size;
	cl_mem			img_buffer;
	cl_mem			aux_buffer;
	cl_mem			light_buffer;
	cl_mem			obj_buffer;
}					t_gpu_mem;

typedef struct		s_mlx
{
	void			*mlx_ptr;
	void			*win;
	t_img			img;
	int				h;
	int				len;
	t_cl_arg		cl;
	t_screen		screen;
	t_cam			cam;
	t_counter		counter;
	t_object		*obj;
	t_light			*light;
	t_mouse			mouse;
	t_obj_texture	*texture;
	cl_uint 		objects_count;
	t_gpu_mem		*gpu_mem;
	int				*aux;
}					t_mlx;


int					check_key(int keycode, t_mlx *mlx);
int					ft_esc(t_mlx *mlx);
int					ft_move_x(int keycode, t_mlx *mlx);
int					ft_move_y(int keycode, t_mlx *mlx);
int					ft_move_alpha(int keycode, t_mlx *mlx);
int					ft_move_betta(int keycode, t_mlx *mlx);
int					ft_move_z(int keycode, t_mlx *mlx);


int					mouse_press(int button, int x, int y, t_mlx *mlx);
int					mouse_move(int x, int y, t_mlx *mlx);
int					mouse_release(int button, int x, int y, t_mlx *mlx);
int					ft_move_mouse(t_mlx *mlx, int button, int x, int y);
int					ft_mouse_alpha_betta(t_mlx *mlx, int x, int y);
int					ft_mouse_x_y(t_mlx *mlx, int x, int y);

cl_float3			cl_sum(cl_float3 v1, cl_float3 v2);
cl_float3			cl_minus(cl_float3 v1, cl_float3 v2);
float				cl_length(cl_float3 v);
cl_float3			cl_mult_n(cl_float3 v1, float n);
void				cl_to_norm(cl_float3 *v);
cl_float3			cl_normalize(cl_float3 v1);
cl_float3			cl_cross(cl_float3 v1, cl_float3 v2);
float				cl_dot(cl_float3 v1, cl_float3 v2);
float				cl_angle(cl_float3 v1, cl_float3 v2);


void 				draw_picture(t_mlx *mlx);
int					cl_worker(t_mlx *mlx);
cl_device_id		create_device();
cl_program			build_program(cl_context ctx, cl_device_id dev, const char* filename);
int					read_map(char *name, t_mlx *mlx);


cl_float3			spherical(float phi, float tetta);
cl_float3			spher_norm(cl_float3 v);
void				calc_screen(t_screen *screen, t_cam *cam);

void				get_textures(t_mlx *mlx, char **texture_file, int number_of_texture);
void				find_textures_size(t_mlx *mlx, char **texture_file, int number_of_texture);
void				fill_gpu_mem(t_mlx *mlx);


#endif
