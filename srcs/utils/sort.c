/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sort.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:16:26 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:16:29 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	uses quicksort to sort environment variables.
**	divides array into 2 parts using 2 iterators, one starting from left and
**	one starting from right, until they meet in middle.
**	left and right partition contain correct but unsorted groups of elements,
**	Recursive calls to quicksort then sort them.
*/

void	swap(char **array, int i, int j)
{
	char *temp;

	temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

int		env_cmp(const char *str1, const char *str2)
{
	int		i;

	i = 0;
	while ((str1[i] && str2[i]) && str1[i] == str2[i] &&
			str1[i] != '=' && str2[i] != '=')
		i++;
	if (!str1[i] && !str2[i])
		return (0);
	if (str1[i] == '=' && str2[i] != '=')
		return (0 - str2[i]);
	else if (str2[i] == '=' && str1[i] != '=')
		return (str1[i] - 0);
	return (str1[i] - str2[i]);
}

void	quicksort(char **array, int start, int end)
{
	int		i;
	int		j;
	char	*pivot;

	i = start;
	j = end;
	if (j - i < 1)
		return ;
	pivot = array[i];
	while (j > i)
	{
		while (env_cmp(array[i], pivot) <= 0 && i < end && j > i)
			i++;
		while (env_cmp(array[j], pivot) >= 0 && j > start && j >= i)
			j--;
		if (j > i)
			swap(array, i, j);
	}
	swap(array, start, j);
	quicksort(array, start, j - 1);
	quicksort(array, j + 1, end);
}

void	sort_env(char **array, int start, int length)
{
	if (length <= 1 || start >= length)
		return ;
	quicksort(array, start, length - 1);
	return ;
}
