/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   escape_characters.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:39:43 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/04 17:35:19 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool			escape_characters(t_shell **shell, char **env_str,
		unsigned char *parse)
{
	int		to_escape;
	int		len;
	int		new_len;
	char	*new_env_str;

	to_escape = 0;
	len = 0;
	new_env_str = NULL;
	new_len = 0;
	count_esc_char(env_str, &to_escape, &len, parse);
	if (!to_escape)
		return (false);
	new_env_str = malloc((len + to_escape + 1) * sizeof(char));
	ft_bzero(new_env_str, len + to_escape);
	if (!new_env_str)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	len = 0;
	expand_esc_char(env_str, &new_env_str, &new_len, parse);
	new_env_str[new_len] = '\0';
	*env_str = new_env_str;
	return (true);
}

void			count_esc_char(char **env_str, int *to_escape, int *len,
		unsigned char *parse)
{
	while ((*env_str)[*len])
	{
		if ((*env_str)[*len] == '\\'
				|| ((*env_str)[*len] == '\'' && !(*parse & QUOTED_DOUBLE))
				|| (*env_str)[*len] == '"'
				|| ((!(*parse & QUOTED))
					&& ft_strchr("<>|;", (*env_str)[*len])))
			(*to_escape)++;
		(*len)++;
	}
}

void			expand_esc_char(char **env_str, char **new_env_str,
		int *new_len, unsigned char *parse)
{
	int	len;

	len = 0;
	while ((*env_str)[len])
	{
		if ((*env_str)[len] == '\\'
				|| ((*env_str)[len] == '\'' && !(*parse & QUOTED_DOUBLE))
				|| (*env_str)[len] == '"'
				|| ((!(*parse & QUOTED)) && ft_strchr("<>|;", (*env_str)[len])))
		{
			(*new_env_str)[*new_len] = '\\';
			(*new_len)++;
		}
		(*new_env_str)[*new_len] = (*env_str)[len];
		len++;
		(*new_len)++;
	}
}
