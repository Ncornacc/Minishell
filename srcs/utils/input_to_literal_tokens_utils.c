/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_to_literal_tokens_utils.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:15:16 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:15:23 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		n_tokens(char *line)
{
	int		i;
	int		tokens;
	int		trim;

	tokens = 0;
	i = 0;
	while (line[i])
	{
		trim = trim_words(line + i);
		if (!trim)
			break ;
		i += trim;
		tokens++;
	}
	return (tokens);
}

bool	is_escaped_meta_token(char *token)
{
	return (match_str("\\>", token, 1) ||
			match_str("\\>\\>", token, 1) ||
			match_str("\\<", token, 1) ||
			match_str("\\;", token, 1) ||
			match_str("\\|", token, 1));
}
