/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_strcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:18:43 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:18:45 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*create_temp_line(char *lit_token, int *i, int index,
		t_shell **shell)
{
	char	*env_str;
	char	*temp_line;
	char	swap;
	char	free_env;

	swap = lit_token[*i];
	free_env = 0;
	lit_token[*i] = '\0';
	env_str = fetch_env(lit_token + index + 1, shell, &free_env);
	if (!env_str)
		return (NULL);
	free_env |= escape_characters(shell, &env_str, &((*shell)->parse));
	temp_line = concat_result(env_str, lit_token + *i, swap, free_env);
	*i = str_len(env_str, NULL, 0) - 1;
	if (free_env)
		free(env_str);
	if (!temp_line)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	return (temp_line);
}

char			*create_lh_string(t_shell **shell, char **input,
		unsigned char *parse, int *home_len)
{
	char	*home;
	char	free_home;
	char	*lh_string;

	free_home = 0;
	home = check_env_str((*shell)->home_env_name, 4, (*shell)->envp_cpy, 0);
	if (!home)
	{
		*home_len = -1;
		return (NULL);
	}
	if (escape_characters(shell, &home, parse))
		free_home = 1;
	*home_len = str_len(home, NULL, 0);
	lh_string = ft_join(*input, home, str_len(*input, NULL, 0) + *home_len);
	if (free_home)
		free(home);
	if (!lh_string)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	return (lh_string);
}

char			*concat_result(char *env_str, char *lit_token, char swap,
		char free_env)
{
	char	*temp_line;

	if (swap || free_env)
	{
		*lit_token = swap;
		temp_line = ft_join(env_str, lit_token, str_len(env_str, NULL, 0)
				+ str_len(lit_token, NULL, 0));
	}
	else
		temp_line = ft_join(env_str, NULL, str_len(env_str, NULL, 0));
	return (temp_line);
}

void			update_input(char **input, char **lh_string, char **rh_string,
		int size)
{
	free(*input);
	*input = ft_join(*lh_string, *rh_string, size);
	free(*lh_string);
	free(*rh_string);
}
