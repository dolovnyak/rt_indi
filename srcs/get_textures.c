#include "rt.h"
#define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

static int				sdl_log_error(const char *p, const int id)
{
	printf("%s ----> ERROR <---- %s\n", KRED, KNRM);
	printf("INCORRECT: %s%s%s%s%s\n",
			p,
			id < 0 ? "" : " IN ID = ",
			KGRN,
			id < 0 ? "" : ft_itoa(id),
			KNRM);
	return (FUNCTION_FAILURE);
}

void	find_textures_size(t_mlx *mlx, char **texture_file, int number_of_texture)
{
	unsigned char	*tex_data;
	int 			bpp;
	int 			texture_w;
	int 			texture_h;
	int				i;

	i = -1;
	mlx->texture->texture_size = 0;
	while (++i < number_of_texture)
	{
		if (!(tex_data = stbi_load(texture_file[i], &texture_w,
								   &texture_h, &bpp, 4)))
		{
			sdl_log_error("TEXTURE ERROR OR TEXTURE PATH NOT FOUND", i);
			exit(-1);
		}
		mlx->texture->texture_w[i] = texture_w;
		mlx->texture->texture_h[i] = texture_h - 1;
		mlx->texture->texture_size += (texture_w * texture_h);
		free(tex_data);
	}
}

void get_textures(t_mlx *mlx, char **texture_file, int number_of_texture)
{
	unsigned char	*tex_data;
	int 			x;
	int 			y;
	int 			total_texture_size;
	int 			i;

	i = -1;
	total_texture_size = 0;
	mlx->texture->prev_texture_size[0] = 0;
	while (++i < number_of_texture)
	{
		if (!(tex_data = stbi_load(texture_file[i], &mlx->texture->w,
								   &mlx->texture->h, &mlx->texture->bpp, 4)))
		{
			sdl_log_error("TEXTURE ERROR OR TEXTURE PATH NOT FOUND", i);
			exit(-1);
		}
		y = -1;
		while (++y < mlx->texture->h)
		{
			x = -1;
			while (++x < mlx->texture->w)
			{
				mlx->texture->texture[(x + (y * mlx->texture->w)) + total_texture_size] =
						*((int *) tex_data + x + y * mlx->texture->w);
			}
		}
		mlx->texture->prev_texture_size[i] = total_texture_size;
		total_texture_size += mlx->texture->w * (mlx->texture->h - 1);
		free(tex_data);
	}
}