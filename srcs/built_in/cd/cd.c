/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:21:34 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:21:40 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Errors if more than one argument.
**	If first argument is "-", changes to OLDPWD. If more than '-', flags no_opt.
**	Else if first argument is given, assigns that to dir.
**	Else assigns HOME path to dir. Executes syscall (chdir). Updates env vars.
*/

void	built_in_cd(t_shell **shell)
{
	bool	previous;
	char	*dir;
	char	oldpwd[PATH_MAX];
	t_list	*arg;

	previous = false;
	arg = (*((*shell)->current_command))->args->next;
	if (arg && arg->next)
	{
		error_handler(shell, NULL, ERROR, "too many arguments");
		return ;
	}
	dir = built_in_cd_get_dir(shell, arg, &previous);
	if (!dir)
	{
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		return ;
	}
	if (!arg && built_in_cd_home(shell, dir))
		return ;
	else if (previous && built_in_cd_previous(shell, dir))
		return ;
	built_in_cd_save_and_chdir(shell, dir, oldpwd);
}

int		cd_free_update(t_shell **shell, char *dot_sub_path, size_t new_path_len)
{
	update_pwd(shell, dot_sub_path, new_path_len);
	return (free_paths(NULL, NULL, &dot_sub_path, NULL));
}

int		built_in_cd_save_and_chdir(t_shell **shell, char *dir, char *oldpwd)
{
	char	*new_path;
	char	*dot_sub_path;
	char	*new_path_tmp;
	size_t	new_path_len;

	new_path = create_new_path(shell, dir);
	new_path_len = str_len(new_path, NULL, 0);
	new_path_tmp = create_tmp_path(shell, &new_path, new_path_len, dir);
	oldpwd = getcwd(oldpwd, PATH_MAX);
	dot_sub_path = dot_sub(new_path_tmp, new_path_len);
	if (!dot_sub_path && free_paths(&new_path, &new_path_tmp, NULL, dir))
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	if (*dir && chdir(dot_sub_path) == -1)
	{
		error_handler(shell, dir, ERROR, strerror(errno));
		if (!oldpwd && is_relative_dot(dir))
		{
			update_pwd(shell, new_path, new_path_len);
			(*shell)->current_status = SUCCESS;
		}
		return (free_paths(&new_path, &new_path_tmp, &dot_sub_path, dir));
	}
	new_path_len = str_len(dot_sub_path, NULL, 0);
	free_paths(&new_path, &new_path_tmp, NULL, dir);
	return (cd_free_update(shell, dot_sub_path, new_path_len));
}
