/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/09 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_comm **lst)
{
	int		len;
	int		location;
	t_envp	*prev;

	prev = (*lst)->e;
	printf("|%s|\n", (*lst)->e->key);
	if (prev)
	{
		len = (int)ft_listlen(prev);
		printf("%s len - %d\n", RED, len);
		location = locate_env_key((*lst)->e, (*lst)->cmd[1], 0);
		if (location == -1 || len <= 0 || location + 1 > len)
			return (EXIT_FAILURE);
		printf("%s locat - %d\n", GREEN, location);
		remove_element(&(*lst)->e, location);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
