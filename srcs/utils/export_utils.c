/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/05 19:14:52 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/05 19:15:16 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		built_in_export_escape_char(char *envp)
{
	int	len;

	len = 0;
	while (envp[len])
	{
		if (envp[len] == '"' || envp[len] == '\\' || envp[len] == '$')
		{
			if (len)
				write(STDOUT_FILENO, envp, len);
			write(STDOUT_FILENO, "\\", 1);
			write(STDOUT_FILENO, envp + len, 1);
			return (len + 1);
		}
		len++;
	}
	write(STDOUT_FILENO, envp, len);
	return (len);
}

void	built_in_export_display_env(char *envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i += built_in_export_escape_char(envp + i);
}
