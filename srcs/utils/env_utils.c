/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:14:33 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:14:36 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	env_cpy is copied and allocated when entering the shell.
**	Until we exit it will be modified.
**	the number of variables is stored (n_env) and its size too(env_cpy_size)
*/

int		n_env(char **envp)
{
	int	n;

	n = 0;
	while (envp[n])
		n++;
	return (n);
}

/*
**	searches environment variables to check if existing
*/

int		match_env(char *current, char *s2)
{
	if (!current || !s2)
		return (false);
	while (*current && *s2 && *current == *s2)
	{
		if (*current == '=')
			return (true);
		current++;
		s2++;
	}
	if ((!*current || *current == '=') && (!*s2 || *s2 == '='))
		return (true);
	return (false);
}

/*
**	changes value of environment variable
*/

int		change_env(t_shell **shell, char **envp, char *name, int internal)
{
	int len;

	if (!ft_strchr(name, '='))
		return (SUCCESS);
	len = str_len(name, NULL, 0);
	free(*envp);
	*envp = ft_join(name, NULL, len);
	if (!(*envp))
	{
		if (internal)
			return (ERROR);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	return (SUCCESS);
}
