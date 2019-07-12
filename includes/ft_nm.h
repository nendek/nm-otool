/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnardozi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:21:23 by pnardozi          #+#    #+#             */
/*   Updated: 2019/07/12 12:21:25 by pnardozi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ranlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>

# define OUT_64 2
# define OUT_32 1
# define PRINT 1
# define NO_PRINT 2
# define LIB_MAGIC 0xA3E686372613C21
# define PROT_RD 1
# define NO_PROT_RD 2

typedef struct				s_symb
{
	size_t					val;
	char					symb;
	char					reserved[7];
	char					*name;
	struct s_symb			*left;
	struct s_symb			*right;
}							t_symb;

typedef struct				s_env
{
	void					*file;
	char					*filename;
	t_symb					*lst;
	uint8_t					text_sect;
	uint8_t					data_sect;
	uint8_t					bss_sect;
	uint8_t					index;
	uint32_t				offset;
	struct load_command		*current_lc;
	struct symtab_command	*sym;
	void					*endfile;
	char					*print_buff;
	size_t					index_buff;
}							t_env;

void						nm(void *file, char *filename, size_t size_file);
t_env						*init_env(void *file, char *filename,
		size_t size_file);
void						set_env(t_env *env, uint32_t offset);
void						*move_ptr(t_env *env, void *ptr, size_t to_add);
void						free_tree(t_symb *node);

uint32_t					find_cpu_arch(t_env *env);
uint32_t					find_cpu_subtype_arch(t_env *env, uint32_t index);
uint32_t					find_cpu_arch_nb(t_env *env);
uint32_t					get_size_lib(t_env *env, uint32_t nb);
char						check_magic(t_env *env);

void						handle_fat_cigam(t_env *env);
void						handle_lib(t_env *env, uint32_t size);
void						handle_64(t_env *env, uint32_t (*f32)(uint32_t));
void						handle_32(t_env *env, uint32_t (*f)(uint32_t));

/*
 ** Utils functions
*/

void						print_one_arch(t_env *env, uint32_t nb);
void						print_filename(t_env *env, uint32_t nb,
		uint32_t magic, uint32_t index);
void						print_object_name(t_env *env, char *name);
void						print_list(t_env *env, t_symb *lst, char type);
void						print_buff(t_env *env);
void						ft_putval(t_env *env, size_t val, char type);
void						handle_buff(t_env *env, char c);

int							ft_strcmp(t_env *env, char *s1, char *s2);
size_t						ft_atoi(t_env *env, char *str);
void						ft_putstr(t_env *env, char *str, char prot);
size_t						ft_strlen(t_env *env, char *string, char prot);
char						*get_name(t_env *env, char *string, char prot);
size_t						get_name_len(t_env *env, char print);
void						ft_push(t_env *env, t_symb **beg, t_symb *tmp);

uint32_t					same_32(uint32_t val);
uint64_t					same_64(uint64_t val);
uint32_t					indian_32(uint32_t val);
uint64_t					indian_64(uint64_t val);

#endif
