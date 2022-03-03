/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 13:21:45 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../includes/minishell.h"

static inline long long int	ft_exit_atoi(char *str, int *sign, int n)
{	
	int	i;

	i = 0;
	if (str && str[i] == '-')
	{
		(*sign) = -1;
		i++;
	}
	while (str && ft_isdigit(str[i]) && (*sign) != -1)
	{
		n = 10 * n + (str[i] - 48);
		i++;
	}
	if ((*sign) == -1)
	{
		n = ft_atoi(str);
		g_error_status = ((unsigned long long)n);
		exit (g_error_status);
	}
	return (n);
}

static inline int	ft_new_value_error(char *str)
{
	int					sign;
	unsigned long long	n;
	unsigned long long	max;

	max = 9223372036854775807;
	sign = 1;
	n = ft_exit_atoi(str, &sign, n);
	if (n > max && sign != -1)
	{
		write(2, "bash: exit: ", 12);
		write(2, str, ft_strlen(str));
		write(2, " numeric argument required\n", 28);
		g_error_status = 255;
		return (g_error_status);
	}
	n = n % 256;
	while (n < 0)
		n = n + 256;
	return (n);
}

static inline int	ft_check_exit_numeric(t_comm *lst)
{
	int		count_min;
	int		j;

	j = 0;
	count_min = 0;
	while (lst->cmd[1][j])
	{
		if (lst->cmd[1][j] == '-' && count_min == 0)
		{
			count_min = 1;
			j++;
		}
		if (!(ft_isdigit(lst->cmd[1][j])))
		{
			write(2, "bash: exit: ", 12);
			write(2, lst->cmd[1], ft_strlen(lst->cmd[1]));
			write(2, ": numeric argument required\n", 29);
			g_error_status = 255;
			exit (g_error_status);
		}
		j++;
	}
	g_error_status = ft_new_value_error(lst->cmd[1]);
	return (g_error_status);
}

static inline int	ft_exit_many_args(t_comm *lst)
{
	int	i;

	i = 0;
	while (lst->cmd[1][i])
	{
		if (ft_isalpha(lst->cmd[1][i]))
		{
			write(2, "bash: exit: ", 12);
			write(2, lst->cmd[1], ft_strlen(lst->cmd[1]));
			write(2, ": numeric argument required\n", 29);
			g_error_status = 255;
			exit (g_error_status);
		}
		i++;
	}
	write(2, "bash: exit: ", 12);
	write(2, ": too many arguments\n", 22);
	g_error_status = 1;
	return (g_error_status);
}

int	ft_exit(t_comm *lst)
{
	int		count;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	count = ft_count_strings(lst);
	if (count == 2)
		g_error_status = ft_check_exit_numeric(lst);
	else if (count > 2)
		g_error_status = ft_exit_many_args(lst);
	else
		exit (0);
	return (EXIT_FAILURE);
}
