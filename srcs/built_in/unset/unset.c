/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/15 19:49:26 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/03 21:42:55 by mjiam         ########   odam.nl         */
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
