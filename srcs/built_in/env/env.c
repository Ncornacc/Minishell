/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:21:50 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:21:52 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Flags error if option or arguments are found.
**	Else prints variables with values.
*/

void	built_in_env(t_shell **shell)
{
	int		env;
	t_list	*arg;

	env = 0;
	arg = (*((*shell)->current_command))->args->next;
	if (arg && *(arg->name) == '-')
	{
		error_handler(shell, arg->name, ERROR, ERR_NO_OPT);
		return ;
	}
	else if (arg)
	{
		error_handler(shell, arg->name, ERROR, ERR_NO_ARG);
		return ;
	}
	while ((*shell)->envp_cpy[env])
	{
		if (ft_strchr((*shell)->envp_cpy[env], '='))
		{
			ft_putstr_fd((*shell)->envp_cpy[env], STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		env++;
	}
}
