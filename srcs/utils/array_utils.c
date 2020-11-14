/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   array_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/19 19:52:52 by mjiam         #+#    #+#                 */
/*   Updated: 2020/10/19 19:53:29 by mjiam         ########   odam.nl         */
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
