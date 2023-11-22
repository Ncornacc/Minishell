/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   literal_tokens_to_interpreted_tokens.c             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:20:01 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:20:03 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			**create_interpreted_tokens(char **lit_tokens, int n,
		t_shell **shell, int lexer)
{
	char	**int_tokens;

	int_tokens = malloc((n + 1) * sizeof(char *));
	if (!int_tokens)
		return (NULL);
	int_tokens[n] = NULL;
	n = 0;
	while (lit_tokens[n])
	{
		int_tokens[n] = new_interpreted_token(lit_tokens + n, shell, lexer);
		if (lexer && (*shell)->current_status == IGN_ENV_SUB)
			ignore_env_substitution(shell, lit_tokens, int_tokens, n);
		if (!(int_tokens[n]))
		{
			free_array(int_tokens);
			return (NULL);
		}
		if ((*shell)->current_status == BACKSLASH_TERM &&
				bslash_term(shell, lit_tokens, int_tokens, n) == ERROR)
			return (NULL);
		n++;
	}
	if ((*shell)->current_status == IGN_ENV_SUB)
		(*shell)->current_status = SUCCESS;
	return (int_tokens);
}

char			*new_interpreted_token(char **literal_token, t_shell **shell,
		int lexer)
{
	int		prev_size;
	char	*literal_token_copy;
	char	*result;

	prev_size = str_len(*literal_token, NULL, 0);
	literal_token_copy = ft_join(*literal_token, NULL, prev_size);
	if (!literal_token_copy)
	{
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		return (NULL);
	}
	literal_to_interpreted(&literal_token_copy, shell, lexer);
	if (lexer && (*shell)->current_status == IGN_ENV_SUB)
	{
		free(literal_token_copy);
		return (NULL);
	}
	if ((*shell)->current_status != SUCCESS
			&& (*shell)->current_status != BACKSLASH_TERM)
		return (free_lit_copy(&literal_token_copy));
	copy_int_token(&result, literal_token_copy, prev_size);
	if (!result)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	return (result);
}

void			literal_to_interpreted(char **new_literal_token,
		t_shell **shell, int lexer)
{
	unsigned char	parse;
	int				i;
	int				n_quotes;

	i = 0;
	parse = lexer;
	n_quotes = 0;
	(*shell)->current_status = SUCCESS;
	while ((*new_literal_token)[i])
	{
		if ((*new_literal_token)[i] == '"' || (*new_literal_token)[i] == '\'')
			n_quotes += handle_quotes(new_literal_token, i, &parse, 1);
		if ((*new_literal_token)[i] == '\\' && !(parse & LITERAL_BACKSLASH))
		{
			handle_bslash(shell, new_literal_token, i, &parse);
			if ((*shell)->current_status != SUCCESS)
				return ;
		}
		reset_parse_bslash(new_literal_token, i, &parse);
		if (detect_ignored_env(shell, new_literal_token, i, parse))
			return ;
		reset_parse_literal_env(new_literal_token, i, &parse);
		i++;
	}
	flag_quotes(n_quotes, shell);
}

char			*final_interpreted_token(int prev_size, int new_size,
		char *literal_token_copy)
{
	char	*token;
	int		i;

	i = 0;
	token = malloc((new_size + 1) * sizeof(char));
	if (!token)
		return (NULL);
	token[new_size] = '\0';
	new_size = 0;
	while (i < prev_size)
	{
		if (literal_token_copy[i])
		{
			token[new_size] = literal_token_copy[i];
			new_size++;
		}
		i++;
	}
	return (token);
}

int				calculate_new_size(char *line, int prev_size)
{
	int	i;
	int	new_size;

	i = 0;
	new_size = prev_size;
	while (i < prev_size)
	{
		if (!line[i])
			new_size--;
		i++;
	}
	return (new_size);
}
