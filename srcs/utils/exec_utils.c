/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:14:43 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:14:45 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  calls dup2 on stdin/out and closes process in/out fd
*/

void	dup2_and_close(t_shell **shell, int fd, int stdio)
{
	if (dup2(fd, stdio) == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	if (close(fd) == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
}

/*
**  opens in/output files, assigns fd, and flags error if any
*/

int		open_file(t_shell **shell, t_list **current, int *fd, int mode)
{
	if (mode)
		*fd = open((*current)->name, (*current)->value, 0644);
	else
		*fd = open((*current)->name, (*current)->value);
	if (*fd == -1)
	{
		error_handler(shell, (*current)->name, SHELL_ERROR, strerror(errno));
		return (ERROR);
	}
	return (SUCCESS);
}
