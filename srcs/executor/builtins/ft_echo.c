/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/20 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_comm *lst)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (lst->cmd[i] && !ft_strncmp(lst->cmd[i], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (lst->cmd[i])
	{
		ft_putstr_fd(lst->cmd[i], 1);
		if (lst->cmd[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (n_flag == 0)
		ft_putchar_fd('\n', 1);
	g_error_status = 0;
	return (EXIT_SUCCESS);
}
