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
