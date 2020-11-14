/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_arg_exp.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 18:00:20 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/04 18:00:22 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_args_var_expansion(t_shell **shell)
{
	t_list **tmp;

	(*shell)->current_status = SUCCESS;
	tmp = (*((*shell)->current_command))->current_arg;
	while (*tmp)
	{
		(*shell)->current_status = SUCCESS;
		if ((*tmp)->token_type == LITERAL_TOKEN)
		{
			if (cmd_args_expand_tokens(shell, &tmp) == STOP)
				return ;
		}
		else
			tmp = &((*tmp)->next);
	}
}

int		cmd_args_expand_tokens(t_shell **shell, t_list ***tmp)
{
	char	*expand;
	char	**lit_tokens;
	char	**int_tokens;

	int_tokens = NULL;
	lit_tokens = NULL;
	expand = NULL;
	cmd_args_expand_literal(shell, &lit_tokens, &expand, *tmp);
	if ((*shell)->current_status != SUCCESS)
		return (STOP);
	if (!lit_tokens[0])
		return (cmd_args_update_args_lit(shell, tmp, &expand, &lit_tokens));
	if (cmd_args_get_int_tokens(shell, &lit_tokens, &int_tokens) == STOP)
		return (free_tmp_tokens(&expand, &lit_tokens, NULL));
	if (int_tokens && !int_tokens[0])
	{
		cmd_args_update_current_arg_int(shell, tmp);
		free_tmp_tokens(&expand, &lit_tokens, &int_tokens);
		cmd_args_loop_expand(shell, *tmp);
		return (STOP);
	}
	cmd_args_push_arg_list(shell, int_tokens, tmp);
	free_tmp_tokens(&expand, &lit_tokens, &int_tokens);
	return (SUCCESS);
}

void	cmd_args_expand_literal(t_shell **shell, char ***literal_result,
		char **expand, t_list **tmp)
{
	*expand = ft_join((*tmp)->name, NULL, str_len((*tmp)->name, NULL, 0));
	input_to_literal_input(shell, expand, 0);
	if ((*shell)->current_status != SUCCESS)
	{
		free(*expand);
		return ;
	}
	*literal_result = create_literal_tokens(expand, shell);
}

int		cmd_args_update_args_lit(t_shell **shell, t_list ***tmp, char **expand,
		char ***lit_tokens)
{
	t_list	*next;

	if (!(*(*tmp))->next)
	{
		free((**tmp));
		**tmp = NULL;
		(*((*shell)->current_command))->current_arg =
			&((*((*shell)->current_command))->args);
		free_tmp_tokens(expand, NULL, lit_tokens);
		return (STOP);
	}
	else
	{
		next = (*(*tmp))->next;
		(*((*shell)->current_command))->current_arg = *tmp;
		free(*((*((*shell)->current_command))->current_arg));
		*((*((*shell)->current_command))->current_arg) = next;
		*tmp = (*((*shell)->current_command))->current_arg;
	}
	free_tmp_tokens(expand, NULL, lit_tokens);
	if (!(**tmp))
		return (STOP);
	cmd_args_var_expansion(shell);
	return (STOP);
}

int		cmd_args_get_int_tokens(t_shell **shell, char ***lit_tokens,
		char ***int_tokens)
{
	*int_tokens = create_interpreted_tokens(*lit_tokens,
			(*shell)->current_n_tokens, shell, 0);
	if (!*int_tokens)
	{
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		return (STOP);
	}
	return (SUCCESS);
}
