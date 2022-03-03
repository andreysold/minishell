/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                            :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 14:01:23 by galetha          ###   ########.fr       */
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
	return (EXIT_SUCCESS);
}
