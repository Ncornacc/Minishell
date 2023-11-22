/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_env_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:17:07 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:17:09 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		built_in_cd_home(t_shell **shell, char *dir)
{
	if (*dir && chdir(dir) == -1)
	{
		error_handler(shell, dir, ERROR, strerror(errno));
		free(dir);
		dir = NULL;
		return (true);
	}
	if (*dir)
		update_pwd(shell, dir, str_len(dir, NULL, 0));
	free(dir);
	return (true);
}

int		built_in_cd_previous(t_shell **shell, char *dir)
{
	if (*dir && chdir(dir) == -1)
	{
		error_handler(shell, dir, ERROR, strerror(errno));
		free(dir);
		return (true);
	}
	if (*dir)
		update_pwd(shell, dir, str_len(dir, NULL, 0));
	ft_putstr_fd(dir, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	free(dir);
	return (true);
}

void	update_pwd_get_tmp(t_shell **shell, char **tmp)
{
	int	i;

	i = 0;
	while ((*shell)->envp_cpy[i])
	{
		if (match_env((*shell)->envp_cpy[i], "PWD"))
			*tmp = (*shell)->envp_cpy[i] + str_len("PWD", NULL, 0) + 1;
		i++;
	}
}

void	update_pwd(t_shell **shell, char *new_path, size_t new_path_len)
{
	char	*new_pwd;
	char	*tmp;
	char	*new_oldpwd;

	new_oldpwd = NULL;
	tmp = NULL;
	update_pwd_get_tmp(shell, &tmp);
	get_new_old_pwd(shell, tmp, &new_oldpwd, new_path);
	new_pwd = ft_join("PWD=", new_path, 4 + new_path_len);
	if (!new_pwd && free_paths(NULL, NULL, NULL, new_path))
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	if (built_in_export_with_arg(shell, new_pwd, 1) == ERROR
			&& free_paths(&new_pwd, NULL, NULL, new_path))
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	if ((*shell)->saved_pwd)
		free((*shell)->saved_pwd);
	(*shell)->saved_pwd = NULL;
	(*shell)->saved_pwd = ft_join(new_path, NULL, new_path_len);
	if (!(*shell)->saved_pwd && free_paths(&new_pwd, NULL, NULL, new_path))
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	free(new_pwd);
}
