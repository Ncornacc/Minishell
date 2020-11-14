/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_name_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 15:50:05 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/04 17:34:44 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_name_update_name(t_shell **shell, char **interpreted_result)
{
	(*((*shell)->current_command))->name = interpreted_result[0];
	(*((*shell)->current_command))->token_type = EXPANDED_TOKEN;
	(*((*shell)->current_command))->args->name = interpreted_result[0];
	(*((*shell)->current_command))->args->token_type = EXPANDED_TOKEN;
}

void	cmd_name_push_arg_list(t_shell **shell, char **interpreted_result)
{
	int		arg;
	t_list	*node;
	t_list	*list;
	t_list	**current;

	arg = 1;
	list = NULL;
	node = NULL;
	while (interpreted_result[arg])
	{
		if (list)
			update_node(shell, &node, interpreted_result[arg]);
		else
			update_list(shell, &list, interpreted_result[arg], &current);
		update_current(&current, &node);
		arg++;
	}
	if (list)
	{
		if ((*((*shell)->current_command))->args)
			(*((*shell)->current_command))->current_arg = &((*current)->next);
		push_to_list(&((*((*shell)->current_command))->args), list);
	}
	(*((*shell)->current_command))->n_args += arg;
}

char	*cmd_name_cpy_name(t_shell **shell)
{
	char *cpy;

	cpy = ft_join((*((*shell)->current_command))->name, NULL,
			str_len((*((*shell)->current_command))->name, NULL, 0));
	if (!cpy)
	{
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		return (NULL);
	}
	return (cpy);
}
