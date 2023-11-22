/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_to_literal_input.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:19:22 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:19:24 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			input_to_literal_input(t_shell **shell, char **input, int lexer)
{
	int				n_quotes;
	int				i;

	(*shell)->parse = lexer;
	n_quotes = 0;
	i = 0;
	while ((*input)[i])
	{
		if (((*input)[i] == '"' || (*input)[i] == '\''))
			n_quotes += handle_quotes(input, i, &((*shell)->parse), 0);
		if (parse_literal_input(shell, input, &i, &((*shell)->parse)) == ERROR)
		{
			if ((*shell)->current_status == IGN_ENV_SUB)
				(*shell)->current_status = SUCCESS;
			return ;
		}
		i++;
	}
	if (n_quotes % 2)
		error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
	if ((*shell)->current_status == IGN_ENV_SUB)
		(*shell)->current_status = SUCCESS;
}

int				parse_literal_input(t_shell **shell, char **input, int *i,
		unsigned char *parse)
{
	if ((*input)[*i] == '\\')
	{
		handle_bslash_input(shell, input, *i, parse);
		if ((*parse & LEXER) && (*shell)->current_status == BACKSLASH_TERM)
			return (ERROR);
	}
	if ((*input)[*i] == '$' && (*input)[*i + 1]
			&& !(*parse & LITERAL_ENV) && !(*parse & QUOTED_SINGLE))
	{
		if (handle_env(shell, input, i, parse) == ERROR)
			return (ERROR);
	}
	else if (!(*parse & LEXER) && !*i && (*input)[*i] == '~'
			&& !((*shell)->tokens_info[*i] & QUOTED))
		handle_tilde(input, i, shell, parse);
	else
		reset_literals(input, *i, parse);
	return (SUCCESS);
}

void			integrate_env(char **input, int *i, t_shell **shell)
{
	char	*rh_string;
	int		len;
	int		index;
	char	*new_input;

	index = *i;
	get_offset(shell, input, index, i);
	if ((*shell)->current_status != SUCCESS)
		return ;
	rh_string = create_temp_line(*input, i, index, shell);
	if ((*shell)->current_status != SUCCESS)
		return ;
	(*input)[index] = '\0';
	len = str_len(*input, NULL, 0);
	*i += len;
	len += str_len(rh_string, NULL, 0);
	new_input = ft_join(*input, rh_string, len);
	if (!new_input)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	free(*input);
	*input = new_input;
	free(rh_string);
}

void			reset_literals(char **input, int i, unsigned char *parse)
{
	if (i && (*input)[i - 1] == ' ' && (*parse & LITERAL_WHITESPACE))
		*parse ^= LITERAL_WHITESPACE;
	if (i > 0 && (*input)[i] && (*input)[i] == '$' && (*parse & LITERAL_ENV))
		*parse ^= LITERAL_ENV;
}
