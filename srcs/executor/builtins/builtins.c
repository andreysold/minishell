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

int	check_builtin(t_comm **lst, char **env)
{
	if (ft_strncmp(*(*lst)->cmd, "echo", 5) == 0)
	{
		return (ft_echo(*lst));
	}
	else if (ft_strncmp(*(*lst)->cmd, "cd", 3) == 0)
	{
		return (ft_cd(*lst));
	}
	else if (ft_strncmp(*(*lst)->cmd, "pwd", 4) == 0)
	{
		return (ft_pwd(*lst));
	}
	else if (ft_strncmp(*(*lst)->cmd, "export", 7) == 0)
	{
		return (ft_export(*lst));
	}
	else if (ft_strncmp(*(*lst)->cmd, "unset", 6) == 0)
	{
		return (ft_unset(lst));
	}
	else if (ft_strncmp(*(*lst)->cmd, "env", 4) == 0)
	{
		return (ft_env(*lst));
	}
	else if (ft_strncmp(*(*lst)->cmd, "exit", 5) == 0)
	{
		return (ft_exit(*lst));
	}
	return (-1);
}

int	builtins(t_comm **lst, char **env)
{
	int	bool;

	if ((*lst)->cmd)
	{
		bool = check_builtin(lst, env);
		if (bool == EXIT_SUCCESS)
		{
			return (EXIT_SUCCESS);
		}
		else if (bool == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (-1);
}
