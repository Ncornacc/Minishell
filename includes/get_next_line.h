/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:22:30 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:22:34 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# include <stdlib.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdint.h>

typedef struct	s_data
{
	char			*str;
	struct s_data	*next;
}				t_data;

int				ft_exit(t_data **root, char **bf, char **line, int exit);

int				ft_make_line(t_data **rt, t_data *last, char **line);

int				ft_push(t_data **root, t_data *last, char **bf, ssize_t bytes);

int				ft_push_loop(t_data **root, char **bf, char **line, int fd);

int				get_next_line(int fd, char **line);

t_data			*ft_new(char *bf);

t_data			*ft_last(t_data *lst, int find_nl, int push_loop);

char			*ft_join(char *s1, char *s2, size_t len);

size_t			ft_len(char *bf);

#endif
