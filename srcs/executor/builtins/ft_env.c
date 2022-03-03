/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 18:10:00 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_comm *lst)
{
	t_envp	*tmp;

	tmp = lst->e;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(tmp->value, STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	g_error_status = 0;
	return (EXIT_SUCCESS);
}
