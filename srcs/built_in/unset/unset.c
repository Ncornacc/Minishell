/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:22:09 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:22:11 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Flags error if option found.
**	If env is not found, it isn't considered an error.
*/

void	built_in_unset(t_shell **shell)
{
	t_list	*arg;

	arg = (*((*shell)->current_command))->args->next;
	if (arg && *(arg->name) == '-')
	{
		error_handler(shell, arg->name, ERROR, ERR_NO_OPT);
		return ;
	}
	while (arg)
	{
		pop_env(shell, arg->name);
		arg = arg->next;
	}
}
