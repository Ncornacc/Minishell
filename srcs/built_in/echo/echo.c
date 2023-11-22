/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:21:44 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:21:46 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*
**	Checks for -n option, printing newline if not found.
*/

void	built_in_echo(t_shell **shell)
{
	char	opt;
	t_list	*arg;

	arg = (*((*shell)->current_command))->args->next;
	opt = arg ? built_in_echo_has_opt(arg->name) : 0;
	arg = opt ? arg->next : arg;
	while (arg)
	{
		ft_putstr_fd(arg->name, STDOUT_FILENO);
		arg = arg->next;
		if (arg)
			write(STDOUT_FILENO, " ", 1);
	}
	if (!opt)
		write(STDOUT_FILENO, "\n", 1);
}

char	built_in_echo_has_opt(char *line)
{
	if (!line)
		return (false);
	if (!match_str(line, "-n", 0))
		return (false);
	line += 2;
	if (!*line)
		return (true);
	while (*line && *line == 'n')
		line++;
	if (*line)
		return (false);
	return (true);
}
