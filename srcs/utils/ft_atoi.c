/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:14:58 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:15:01 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	whitespaceskip(const char *str, int *i)
{
	if (str[*i] != 32 && (str[*i] < 9 || str[*i] > 13))
		return (0);
	while (str[*i] == 32 || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	return (1);
}

int	ft_atoi(const char *str)
{
	int			sign;
	int			i;
	intmax_t	n;

	i = 0;
	n = 0;
	whitespaceskip(str, &i);
	sign = (str[i] == '-') ? -1 : 1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if (n >= 922337203685477580)
		{
			if (sign == 1)
				return (-1);
			return (0);
		}
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return ((int)n * sign);
}
