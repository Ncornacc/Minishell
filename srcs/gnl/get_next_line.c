/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/12/11 14:00:30 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/04 13:32:10 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_exit(t_data **root, char **bf, char **line, int exit)
{
	t_data	*temp_next;

	if (exit == 0 && !(*root) && line)
		*line = ft_join("", NULL, 0);
	if (exit == -1 && line)
		*line = ft_join("", NULL, 0);
	if (exit == 2 && ft_make_line(root, ft_last(*root, 0, 0), line) == -1)
		return (ft_exit(root, bf, line, -1));
	while (*root)
	{
		temp_next = (*root)->next;
		free((*root)->str);
		free(*root);
		*root = temp_next;
		if (exit == 1)
			break ;
	}
	if (bf && *bf)
	{
		free(*bf);
		*bf = NULL;
	}
	if (line && !*line && !exit)
		return (-1);
	return (exit == 2 ? 0 : exit);
}

int	ft_make_line(t_data **rt, t_data *last, char **line)
{
	t_data	*next;
	char	*dup;

	if (!last || !(*rt))
		return (0);
	while (*rt != last)
	{
		next = (*rt)->next;
		dup = ft_join(next->str, NULL, ft_len(next->str));
		if (!dup)
			return (-1);
		free(next->str);
		next->str = ft_join((*rt)->str, dup, ft_len((*rt)->str) + ft_len(dup));
		if (!next->str)
			return (ft_exit(rt, &dup, NULL, -1));
		free((*rt)->str);
		free((*rt));
		*rt = next;
		free(dup);
	}
	if (line)
		*line = ft_join((*rt)->str, NULL, ft_len((*rt)->str));
	if (line && !*line)
		return (-1);
	return (1);
}

int	ft_push(t_data **root, t_data *last, char **bf, ssize_t bytes)
{
	ssize_t	i;
	t_data	*new;

	i = 0;
	if (bytes <= 0)
		return (bytes);
	*(*bf + bytes) = '\0';
	while (i < bytes)
	{
		new = ft_new(ft_join(*bf + i, NULL, ft_len(*bf + i) + 1));
		if (!new)
			return (-1);
		i += ft_len(*bf + i) + 1;
		if (!*root)
		{
			*root = new;
			last = *root;
		}
		else
		{
			last->next = new;
			last = last->next;
		}
	}
	return (1);
}

int	ft_push_loop(t_data **root, char **bf, char **line, int fd)
{
	t_data	*last;
	int		push;

	last = ft_last(*root, 0, 0);
	while (last->str[ft_len(last->str)] != '\n')
	{
		push = ft_push(root, last, bf, read(fd, *bf, BUFFER_SIZE));
		if (!push)
			return (ft_exit(root, bf, line, 2));
		if (push == -1)
			return (ft_exit(root, bf, line, -1));
		last = ft_last(last, 1, 1);
	}
	return (ft_exit(root, bf, line, ft_make_line(root, last, line)));
}

int	get_next_line(int fd, char **line)
{
	static	t_data	*root = NULL;
	char			*bf;
	int				push;
	t_data			*last;

	bf = NULL;
	if (BUFFER_SIZE <= 0 || fd == -1 || !line)
		return (ft_exit(&root, NULL, line, -1));
	if ((size_t)(BUFFER_SIZE) != BUFFER_SIZE || BUFFER_SIZE == SIZE_MAX)
		return (ft_exit(&root, NULL, line, -1));
	push = ft_make_line(&root, ft_last(root, 1, 0), line);
	if (push)
		return (ft_exit(&root, &bf, line, push));
	bf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!bf)
		return (ft_exit(&root, &bf, line, -1));
	push = ft_push(&root, ft_last(root, 0, 0), &bf, read(fd, bf, BUFFER_SIZE));
	if (!push && root)
		return (ft_exit(&root, &bf, line, 2));
	if (push <= 0)
		return (ft_exit(&root, &bf, line, push));
	last = ft_last(root, 1, 0);
	if (last)
		return (ft_exit(&root, &bf, line, ft_make_line(&root, last, line)));
	return (ft_push_loop(&root, &bf, line, fd));
}
