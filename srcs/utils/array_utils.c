/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   array_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:11:04 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:11:05 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

char	**copy_array(t_shell **shell, char **src, int size)
{
	int		i;
	int		len;
	char	**dst;

	i = 0;
	dst = (char **)malloc(sizeof(char *) * (size + 1));
	if (!dst)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	while (src[i])
	{
		len = str_len(src[i], NULL, 0);
		dst[i] = ft_join(src[i], NULL, len);
		if (!dst[i])
		{
			free_array(dst);
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		}
		i++;
	}
	dst[i] = NULL;
	return (dst);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	if (array)
		free(array);
	array = NULL;
}
