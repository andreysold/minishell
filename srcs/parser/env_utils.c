/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 14:28:37 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_get_key_value(char **massiv, t_envp *tmp, char *clean, int *i)
{
	massiv[(*i)] = ft_strdup(tmp->key);
	clean = massiv[(*i)];
	massiv[(*i)] = ft_strjoin(massiv[(*i)], "=");
	free(clean);
	clean = massiv[(*i)];
	massiv[(*i)] = ft_strjoin(massiv[(*i)], tmp->value);
	if (clean)
		free(clean);
}

int	ft_cnode(t_envp *list_env)
{
	int		i;
	t_envp	*tmp;

	tmp = list_env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
