/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 13:44:00 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_all_list(t_comm *head)
{
	int	i;

	if (!head)
		return ;
	remove_all_list(head->next);
	if (head->here)
		free(head->here);
	if (head->cmd)
	{
		i = 0;
		while (head->cmd[i])
			i++;
		leak_case(i, head->cmd);
	}
	free(head);
}

void	remove_all_env_list(t_envp *head)
{
	if (!head)
		return ;
	remove_all_env_list(head->next);
	if (head->key)
		free(head->key);
	if (head->value)
		free(head->value);
	free(head);
}

void	clean_env(char **env, t_comm *lst)
{
	int	i;
	int	len;

	i = 0;
	if (env)
	{
		while (env[i])
		{
			free(env[i]);
			i++;
		}
		free(env);
	}
}

void	ft_no_malloc(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free (str[i]);
		i++;
	}
	free (str);
}