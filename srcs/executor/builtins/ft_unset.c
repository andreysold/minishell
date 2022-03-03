/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 18:10:51 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_comm **lst)
{
	int		len;
	int		location;
	t_envp	*prev;

	g_error_status = 0;
	prev = (*lst)->e;
	if (prev)
	{
		len = (int)ft_listlen(prev);
		location = locate_env_key((*lst)->e, (*lst)->cmd[1]);
		if (location == -1 || len <= 0 || location + 1 > len)
			return (EXIT_FAILURE);
		remove_element(&(*lst)->e, location);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
