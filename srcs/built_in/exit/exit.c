/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:21:54 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:21:56 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Retrieves correct status to exit with.
**	Sets exit status if given. Flags out of range, invalid, or too many arg.
**	Frees envp_cpy, home path, and command line before exiting.
*/

void	free_shell_component(t_shell **shell)
{
	if ((*shell)->envp_cpy)
		free_array((*shell)->envp_cpy);
	if ((*shell)->home_parent_shell)
		free((*shell)->home_parent_shell);
	free((*shell)->saved_pwd);
}

void	kill_processes(t_shell **shell)
{
	t_cmd	*cmd;

	if ((*shell)->status != INTERNAL_ERROR || !(*shell)->process_level)
		return ;
	cmd = (*shell)->commands;
	if (!cmd)
		return ;
	while (cmd)
	{
		if (cmd->exec & PROCESS_FORKED
				&& kill(cmd->pid, SIGKILL) == -1)
		{
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
			write(STDERR_FILENO, "\n", 1);
		}
		cmd = cmd->next;
	}
}

void	built_in_exit(t_shell **shell)
{
	int		exit_status;
	t_list	*arg;

	if (!shell || !*shell)
		exit(ERROR);
	exit_status = (*shell)->status < 0 ? ERROR : (*shell)->status;
	arg = built_in_exit_fetch_arg(shell);
	if (arg)
	{
		if (!built_in_exit_within_range(shell, arg->name, &exit_status))
			error_handler(shell, arg->name, 255, "numeric argument required");
		if (arg && arg->next && exit_status != 255)
		{
			error_handler(shell, NULL, ERROR, "too many arguments");
			return ;
		}
	}
	kill_processes(shell);
	reset_shell(shell);
	free_shell_component(shell);
	free(*shell);
	exit(exit_status);
}

t_list	*built_in_exit_fetch_arg(t_shell **shell)
{
	t_list	*curr_arg;

	if (!*(*shell)->current_command)
		return (NULL);
	curr_arg = (*((*shell)->current_command))->args;
	if (curr_arg && match_str("exit", curr_arg->name, 1))
	{
		if (!((*((*shell)->current_command))->exec & PROCESS_FORKED))
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (curr_arg->next);
	}
	return (NULL);
}

/*
**	checks if exit argument is a digit.
**	sets exit status to 255 if not.
**	else sets to converted argument and checks if within range.
*/

bool	built_in_exit_within_range(t_shell **shell, char *exit_arg,
									int *exit_status)
{
	int	i;

	i = 0;
	while (exit_arg[i])
	{
		if (exit_arg[i] == '-' && i == 0)
			i++;
		else if (ft_isalnum(exit_arg[i]) != 2)
		{
			*exit_status = 255;
			return (false);
		}
		else
			i++;
	}
	*exit_status = ft_atoi(exit_arg);
	if (*exit_status < 0 || *exit_status > 255)
		error_handler(shell, exit_arg, 255, "out of range");
	return (true);
}
