# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arobion <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/11 17:36:04 by arobion           #+#    #+#              #
#    Updated: 2019/07/11 18:10:18 by arobion          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Weverything

NM = ft_nm
NM_FILES = nm.c utils.c magic_64.c magic_32.c fat_cigam.c archives.c \
		atoi.c string_utils.c print_utils.c putval.c indian_utils.c \
		env_set.c cpu_utils.c print_one_arch.c

OTOOL = ft_otool
OTOOL_FILES = otool.c utils.c magic_64.c magic_32.c indian_utils.c string_utils.c \
			  atoi.c putval.c fat_cigam.c cpu_utils.c print_utils.c print_one_arch.c \
			  archives.c env_set.c


NM_SRCS_DIR = nm_srcs
NM_SRCS = $(addprefix $(NM_SRCS_DIR)/,$(NM_FILES))

OTOOL_SRCS_DIR = otool_srcs
OTOOL_SRCS = $(addprefix $(OTOOL_SRCS_DIR)/,$(OTOOL_FILES))

INCS_DIR = includes
INCS = -I $(INCS_DIR)

NM_OBJS_DIR = nm_objs
NM_OBJS = $(addprefix $(NM_OBJS_DIR)/,$(NM_FILES:%.c=%.o))

OTOOL_OBJS_DIR = otool_objs
OTOOL_OBJS = $(addprefix $(OTOOL_OBJS_DIR)/,$(OTOOL_FILES:%.c=%.o))

.PHONY: all
all: $(NM) $(OTOOL)

$(NM): $(NM_OBJS_DIR) $(NM_OBJS)
		$(CC) $(CFLAGS) $(INCS) -o $(NM) $(NM_OBJS)

$(NM_OBJS_DIR):
		mkdir -p $(NM_OBJS_DIR)

$(NM_OBJS_DIR)/%.o: $(NM_SRCS_DIR)/%.c
		$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

$(OTOOL): $(OTOOL_OBJS_DIR) $(OTOOL_OBJS)
		$(CC) $(CFLAGS) $(INCS) -o $(OTOOL) $(OTOOL_OBJS)

$(OTOOL_OBJS_DIR):
		mkdir -p $(OTOOL_OBJS_DIR)

$(OTOOL_OBJS_DIR)/%.o: $(OTOOL_SRCS_DIR)/%.c
		$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

.PHONY: clean
clean:
		rm -rf nm_objs otool_objs

.PHONY: fclean
fclean: clean
		rm -f $(NM) $(OTOOL)

.PHONY: re
re: fclean all
