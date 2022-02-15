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

int	ft_echo(t_comm *tmp)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while(tmp->command_str[i] && !ft_strncmp(tmp->command_str[i], "-n", 3))
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

int	ft_cd(t_comm *tmp)
{
	char	*path;
	char	*old_path;
	char	*buf;

	buf = NULL;
	old_path = getcwd(buf, 0);
	path = ft_strjoin(old_path, tmp->command_str[1]);
	chdir(path);
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_comm *tmp)
{
//	char *pwd;
//
//	pwd = NULL;
//	pwd = getcwd(pwd, 256);
//	ft_putendl_fd(pwd, 1);
//	free(pwd);
//	return (EXIT_SUCCESS);
	char	*buf;
	char	*pwd;

	buf = NULL;
	pwd = getcwd(buf, 0);
	ft_putstr_fd(pwd, 1);
	write(1, "\n", 1);
	free(buf);
	free(pwd);
	return (0);
}

int check_builtin(t_comm *tmp, char **env)
{
	ft_putstr_fd("builtin detected -- ", 1);
	ft_putendl_fd(tmp->command_str[0],1);
	if (ft_strncmp(*tmp->command_str, "echo", 5) == 0) ///'-n' should work
	{
		return (ft_echo(tmp));
	}
	else if (ft_strncmp(*tmp->command_str, "cd", 3) == 0) ///'only a relative or absolute path'
	{
		return (ft_cd(tmp));
	}
	else if (ft_strncmp(*tmp->command_str, "pwd", 4) == 0)
	{
		return (ft_pwd(tmp));
	}
	else if (ft_strncmp(*tmp->command_str, "export", 7) == 0)
	{
		//ft_export();
	}
	else if (ft_strncmp(*tmp->command_str, "unset", 6) == 0)
	{
		//ft_unset();
	}
	else if (ft_strncmp(*tmp->command_str, "env", 4) == 0) /// 'no arguments'
	{
		//ft_env();
	}
	else if (ft_strncmp(*tmp->command_str, "exit", 5) == 0)
	{
		//ft_exit();
	}
	return (EXIT_FAILURE);
}
