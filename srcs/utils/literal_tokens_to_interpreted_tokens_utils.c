/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   literal_tokens_to_interpreted_tokens_uti           :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:10:54 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/06 11:49:10 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool			detect_ignored_env(t_shell **shell, char **new_literal_token,
		int i, unsigned char parse)
{
	if (!(parse & LEXER))
		return (false);
	if ((*new_literal_token)[i] == '$' && (*new_literal_token)[i + 1]
			&& !(parse & LITERAL_ENV) && !(parse & QUOTED_SINGLE))
	{
		(*shell)->current_status = IGN_ENV_SUB;
		return (true);
	}
	else if (!i && (*new_literal_token)[i] == '~'
			&& !((*shell)->tokens_info[i] & QUOTED))
	{
		(*shell)->current_status = IGN_ENV_SUB;
		return (true);
	}
	return (false);
}

char			*free_lit_copy(char **literal_token_copy)
{
	if (*literal_token_copy)
		free(*literal_token_copy);
	*literal_token_copy = NULL;
	return (NULL);
}

void			copy_int_token(char **result, char *literal_token_copy,
		int prev_size)
{
	int	new_size;

	new_size = calculate_new_size(literal_token_copy, prev_size);
	*result = final_interpreted_token(prev_size, new_size, literal_token_copy);
	free_lit_copy(&literal_token_copy);
}

unsigned char	bslash_term(t_shell **shell, char **literal_tokens,
		char **final_tokens, int token)
{
	if (!literal_tokens[token + 1])
	{
		free_array(final_tokens);
		error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
		return (ERROR);
	}
	if (!final_tokens[token][0])
		final_tokens[token][0] = ' ';
	(*shell)->current_status = SUCCESS;
	return (SUCCESS);
}

void			ignore_env_substitution(t_shell **shell, char **lit_tokens,
		char **int_tokens, int n)
{
	(*shell)->tokens_info[n] = 0;
	(*shell)->tokens_info[n] |= IGN_ENV_SUB;
	int_tokens[n] = ft_join(lit_tokens[n], NULL,
			str_len(lit_tokens[n], NULL, 0));
}
