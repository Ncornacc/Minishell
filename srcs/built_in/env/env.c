/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/15 16:29:47 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/03 21:41:43 by mjiam         ########   odam.nl         */
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
