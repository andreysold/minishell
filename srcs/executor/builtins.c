/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/09 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_echo(t_comm *tmp)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (tmp->command_str[i] && !ft_strncmp(tmp->command_str[i], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (tmp->command_str[i])
	{
		ft_putstr_fd(tmp->command_str[i], 1);
		i++;
	}
	if (n_flag == 0)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}
