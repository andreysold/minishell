/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 19:20:38 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	export_print(t_comm *lst)
{
	t_envp	*tmp;

	tmp = lst->e;
	while (tmp)
	{
		if (tmp->key)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			if (tmp->value)
			{
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(tmp->value, STDOUT_FILENO);
				ft_putchar_fd('\"', STDOUT_FILENO);
			}
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}

static inline void	add_export_value(t_comm *tmp, char **value, int i, int *j)
{
	int	value_start;

	value_start = 0;
	if (tmp->cmd[i][(*j)++])
	{
		value_start = *j;
		while (tmp->cmd[i][*j])
			(*j)++;
		*value = ft_substr(tmp->cmd[i], value_start, (*j) + 1);
	}
	else
		*value = ft_strdup("");
}

static inline int	check_export_arg(t_comm *tmp, int i, char **key,
									char **value)
{
	int	j;

	j = 0;
	while (tmp->cmd[i][j])
	{
		if (j == 0 && (!ft_isalpha(tmp->cmd[i][j]) || tmp->cmd[i][j] == '_'))
			return (EXIT_FAILURE);
		else if (j != 0)
		{
			if (tmp->cmd[i][j] == '=')
			{
				add_export_value(tmp, value, i, &j);
				return (EXIT_SUCCESS);
			}
			else if ((tmp->cmd[i][j + 1] && tmp->cmd[i][j + 1] == '=')
			|| !tmp->cmd[i][j + 1])
				*key = ft_substr(tmp->cmd[i], 0, j + 1);
			else if (ft_isalnum(tmp->cmd[i][j]) == 0)
				return (EXIT_FAILURE);
		}
		j++;
	}
	return (EXIT_SUCCESS);
}

static void	export_key_value(t_comm *tmp, char **key, char **value, int i)
{
	int		location;

	*key = NULL;
	*value = NULL;
	g_error_status = 0;
	if (check_export_arg(tmp, i, key, value))
	{
		g_error_status = 1;
		printf("bash: export: %s: not a valid identifier\n", tmp->cmd[i]);
	}
	if (*key)
	{
		location = locate_env_key(tmp->e, *key);
		if (location == -1)
			add_to_env(tmp->e, *key, *value);
		else
		{
			if (*value)
			{
				upd_env_value(tmp->e, *value, location);
				free(*value);
			}
		}
	}
}

int	ft_export(t_comm *lst)
{
	char	*key;
	char	*value;
	t_comm	*tmp;
	int		i;

	if (lst->cmd[1] == NULL)
		export_print(lst);
	else
	{
		i = 1;
		tmp = lst;
		while (tmp->cmd[i])
		{
			export_key_value(tmp, &key, &value, i);
			if (key)
				free(key);
			if (value)
				free(value);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
