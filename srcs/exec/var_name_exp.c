/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_name_exp.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 18:02:55 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/04 18:02:57 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_name_var_expansion(t_shell **shell)
{
	char	*expand;
	char	**lit_result;
	char	**int_result;

	expand = NULL;
	lit_result = NULL;
	int_result = NULL;
	(*shell)->current_status = SUCCESS;
	if ((*((*shell)->current_command))->token_type == INTERPRETED_TOKEN)
		return ;
	cmd_name_expand_tokens(shell, &lit_result, NULL, &expand);
	if (!lit_result[0])
	{
		cmd_name_shift(shell, lit_result, &expand);
		return ;
	}
	cmd_name_expand_tokens(shell, &lit_result, &int_result, &expand);
	if (!int_result[0])
	{
		cmd_name_shift(shell, int_result, &expand);
		return ;
	}
	cmd_name_update(shell, int_result);
	free(int_result);
	free(expand);
}

void	cmd_name_expand_tokens(t_shell **shell, char ***lit_result,
		char ***int_result, char **expand)
{
	if (!*lit_result)
	{
		*expand = cmd_name_cpy_name(shell);
		input_to_literal_input(shell, expand, 0);
		*lit_result = create_literal_tokens(expand, shell);
		return ;
	}
	if (int_result)
	{
		*int_result = create_interpreted_tokens(*lit_result,
				(*shell)->current_n_tokens, shell, 0);
		free_array(*lit_result);
		*lit_result = NULL;
	}
}

void	cmd_name_shift(t_shell **shell, char **tokens, char **expand)
{
	t_list	*next;

	next = (*((*shell)->current_command))->args->next;
	free((*((*shell)->current_command))->args);
	(*((*shell)->current_command))->args = next;
	(*((*shell)->current_command))->current_arg =
		&((*((*shell)->current_command))->args);
	cmd_name_update_args(shell, next);
	free(tokens);
	free(*expand);
	if ((*((*shell)->current_command))->token_type == LITERAL_TOKEN)
		cmd_name_var_expansion(shell);
}

void	cmd_name_update_args(t_shell **shell, t_list *next)
{
	if (!next)
	{
		(*((*shell)->current_command))->args = NULL;
		(*((*shell)->current_command))->current_arg =
			&((*((*shell)->current_command))->args);
	}
	if ((*((*shell)->current_command))->args)
	{
		(*((*shell)->current_command))->name =
			(*((*shell)->current_command))->args->name;
		(*((*shell)->current_command))->token_type =
			(*((*shell)->current_command))->args->token_type;
	}
	else
	{
		(*((*shell)->current_command))->name = NULL;
		(*((*shell)->current_command))->token_type = INTERPRETED_TOKEN;
	}
}

void	cmd_name_update(t_shell **shell, char **interpreted_result)
{
	cmd_name_update_name(shell, interpreted_result);
	cmd_name_push_arg_list(shell, interpreted_result);
}
