/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa_base.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:26:28 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/03 22:58:35 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_nb_char(int *value, int base)
{
	int	i;
	int	new_value;

	i = 0;
	new_value = *value;
	if ((*value) < 0)
	{
		(*value) *= -1;
		new_value = *value;
		i++;
	}
	while (new_value > 0)
	{
		i++;
		new_value = new_value / base;
	}
	return (i ? i : 1);
}

char	*ft_itoa_base(int value, int base)
{
	int			i;
	char		negative;
	char		*result;
	static char b[16] = "0123456789ABCDEF";

	if (base < 2 || base > 16)
		return (0);
	negative = value < 0;
	i = get_nb_char(&value, base);
	result = malloc((i + 1) * sizeof(char));
	result[i] = '\0';
	while (value >= base)
	{
		i--;
		result[i] = b[value % base];
		value /= base;
	}
	if (negative)
	{
		result[1] = b[value % base];
		result[0] = '-';
	}
	else
		result[0] = b[value % base];
	return (result);
}
