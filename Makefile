#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: broggo <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/07 13:56:12 by broggo            #+#    #+#              #
#    Updated: 2019/09/29 21:37:45 by rkeli            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME	:= 		RT

SRC_DIR := 		./srcs
INC_DIR := 		./includes
OBJ_DIR	:=		./obj
JC_DIR	:=		./obj/rt_jtoc
LIB_DIR	:=		./libft
IMG_DIR :=		./images

CC		:= 		gcc
CFLAGS	:= 		-Wall -Wextra -Werror

SRC		:=		main.c				            \
				check_key_0.c		            \
				check_key_1.c		            \
				check_mouse_0.c		            \
				check_mouse_1.c		            \
				kernel.c			            \
				float3_0.c			            \
				float3_1.c			            \
				cam_and_screen.c	            \
				figures.c                       \
				get_textures.c                  \
				gpu_mem.c						\
				rt_jtoc/rt_jtoc_utilits.c       \
				rt_jtoc/rt_jtoc_get_textures.c  \
				rt_jtoc/rt_jtoc_scene_setup.c  \
				rt_jtoc/rt_jtoc_get_camera.c    \
				rt_jtoc/rt_jtoc_get_objects.c    \
				rt_jtoc/rt_jtoc_get_objects_params.c    \
				rt_jtoc/rt_jtoc_get_lights.c    \
				rt_jtoc/rt_jtoc_get_effects.c    \
				rt_jtoc/rt_jtoc_mouse_setup.c    \
				rt_jtoc/rt_jtoc_get_refl.c    \

SRCS	:=		$(addprefix $(SRC_DIR)/, $(SRC))
OBJ		:= 		$(SRC:.c=.o)
OBJS	:=		$(addprefix $(OBJ_DIR)/, $(OBJ))

FT		:=		./libft/
FT_LIB	:=		$(addprefix $(FT),libft.a)
FT_INC	:=		-I ./libft/includes
FT_LNK	:=		-L ./libft -l ft

CL		:=		./libcl/
CL_LIB	:=		$(addprefix $(FT),libcl.a)
CL_INC	:=		-I ./libcl/include
CL_LNK	:=		-L ./libcl -l cl

JC		:=		./libjtoc/
JC_LIB	:=		$(addprefix $(JC),libjtoc.a)
JC_INC	:=		-I ./libjtoc/include
JC_LNK	:=		-L ./libjtoc -l jtoc


MLX		:=		./minilibx
MLX_LIB	:=		$(addprefix $(MLX),libmlx.a)
MLX_INC	:=		-I ./minilibx
MLX_LNK	:=		-L ./minilibx -l mlx -framework OpenGL -framework AppKit

all:			dirs $(MLX_LIB) $(FT_LIB) $(CL_LIB) $(JC_LIB) $(OBJ_DIR) $(IMG_DIR) $(NAME)

$(FT_LIB):
				make -C $(FT)

$(MLX_LIB):
				make -C $(MLX)

$(JC_LIB):
				make -C $(JC)

$(CL_LIB):
				make -C $(CL)

dirs:			$(OBJ_DIR) $(JC_DIR)

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

$(JC_DIR):
				mkdir -p $(JC_DIR)

$(IMG_DIR):
				mkdir -p $(IMG_DIR)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
				$(CC) $(CFLAGS) -I $(INC_DIR) $(FT_INC) $(CL_INC) $(MLX_INC) $(JC_INC) -o $@ -c $<

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) $(FT_LNK) $(CL_LNK) $(MLX_LNK) $(JC_LNK) $(OBJS) -o $(NAME) -framework OpenCL

clean:
				rm -f $(OBJS)
				make -C $(FT) clean
				make -C $(MLX) clean
				make -C $(JC) clean
				make -C $(CL) clean

fclean: 		clean
				rm -f $(NAME)
				make -C $(FT) fclean
				make -C $(JC) fclean
				make -C $(CL) clean

re: 			fclean all
