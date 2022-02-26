/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/20 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_export_arg(t_comm *copy, int i, char **key, char **value)
{
	int	j;
	int	value_start;

	j = 0;
	value_start = 0;
	while (copy->cmd[i][j])
	{
		if (j == 0 && (ft_isalpha(copy->cmd[i][j]) == 0 || copy->cmd[i][j] == '_'))
			return (EXIT_FAILURE);
		else if (j != 0)
		{
			if (copy->cmd[i][j] == '=')
			{
				if (copy->cmd[i][j++])
				{
					value_start = j;
					while (copy->cmd[i][j])
						j++;
					*value = ft_substr(copy->cmd[i], value_start, j + 1);
					//fixme free
				}
				else
					*value = ft_strdup("");
			}
			else if ((copy->cmd[i][j + 1] && copy->cmd[i][j + 1] == '=') || !copy->cmd[i][j + 1])
				*key = ft_substr(copy->cmd[i], 0, j + 1); //fixme free
			else if (ft_isalnum(copy->cmd[i][j]) == 0)
				return (EXIT_FAILURE);
		}
		j++;
	}
	return (EXIT_SUCCESS);
}

int	ft_export(t_comm *lst) //todo there is no ascii output
{
	int		location;
	char	*key;
	char	*value;
	t_comm	*copy;
	t_envp	*tmp;
	int		i;
	int		j;

	if (lst->cmd[1] == NULL)
	{
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
	else
	{
		i = 1;
		copy = lst;
		while (copy->cmd[i])
		{
			key = NULL;
			value = NULL;
			if (check_export_arg(copy, i, &key, &value))
				bash_error("bash: export: \'", copy->cmd[i], "\': not a valid identifier");
			if (key)
			{
				location = locate_env_key(copy->e, key, 0);
				//todo check if copy may move
				if (location == -1)
					add_to_env(copy->e, key, value, 0);
				else
				{
					if (value)
					{
						upd_env_value(copy->e, value, location, 0);
//						if (value)
//							free(value);
					}
//					else
//					{
////						if (copy->e->value)
////							free(copy->e->value); //убрать значение
//						copy->e->value = NULL;
////						if (value)
////							free(value);
//					}
//					if (key)
//						free(key);
				}
			}
			if (key)
				free(key);
			if (value)
				free(value);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}