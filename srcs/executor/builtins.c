/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/18 17:21:18 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_comm *lst)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while(lst->command_str[i] && !ft_strncmp(lst->command_str[i], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (lst->command_str[i])
	{
		ft_putstr_fd(lst->command_str[i], 1);
		ft_putchar_fd(' ', 1);
		i++;
	}
	if (n_flag == 0)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}

static inline void	upd_oldpwd(const t_comm *lst)
{
	int		location;
	char	*old_path;

	location = locate_env_key(lst->e, "PWD", 1); //updating OLDPWD
	old_path = get_env_value(lst->e, location, 1); //get str for OLDPWD
	if (location == -1) //if there is no OLDPWD1
	{
		add_to_env(lst->e, "OLDPWD", old_path, 1);
	}
	else
	{
		upd_env_value(lst->e, old_path, location, 1);
	}
}

static inline int	search_path(const t_comm *lst, char	**new_path)
{
	int		pos;
	char	*home;

	if (lst->command_str[1] == NULL) //todo check is there NULL
	{
		pos = locate_env_key(lst->e,"HOME", 0);
		if (pos == -1)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(lst->command_str[0], 2);
			ft_putendl_fd(": command not found", 2);
			return (EXIT_FAILURE);
		}
		else
		{
			home = get_env_value(lst->e, pos, 0); //here is PWD
			pos = locate_env_key(lst->e,"PWD", 1);
			upd_env_value(lst->e, home, pos, 1); //todo orig=0
		}
	}
	else if (ft_strncmp(lst->command_str[1], "..", 2) == 0)
	{

	}
	else if (ft_strncmp(lst->command_str[1], "/", 1) == 0) //
	{

	}
	return (EXIT_SUCCESS);
}

/**	copy PATH
 *	add new key-value as OLD_PATH
 *	change PATH to new_path
 *	chdir(new_path) //
**/
int	ft_cd(t_comm *lst)
{
	char	*new_path;
	char	*old_path;
	int		location;
	char	*clean;

	upd_oldpwd(lst);
	if (search_path(lst, &new_path))
		return (EXIT_FAILURE);
	chdir(new_path);
/*	location = locate_env_key(lst->e, "PWD", 1);
	old_path = get_env_value(lst->e, location, 1);
	new_path = ft_strjoin(old_path, "/");
	clean = new_path;
	new_path = ft_strjoin(new_path, lst->command_str[1]);
	free(clean);
	if (location != -1)
		upd_env_value(lst->e, new_path, location, 1);
//	printf("!!! upd val path %s\n", lst->value);
//	fflush(NULL);
	location = locate_env_key(lst->e, "OLDPWD", 1);
	if (location == -1)
		add_to_env(lst->e, "OLDPWD", old_path, 1);
	else
		upd_env_value(lst->e, old_path, location, 1);
	printf("!!! oldpwd %s\n", get_env_value(lst->e, locate_env_key(lst->e,
																   "OLDPWD",
																   1),1));
	fflush(NULL);
	chdir(new_path);
	free(new_path);*/
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_comm *lst)
{
	char	*pwd;
	int		location;

	location = locate_env_key(lst->e, "PWD", 1);
	pwd = get_env_value(lst->e, location, 1);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int ft_env(t_comm *lst)
{
	t_envp	*tmp;

	tmp = lst->e;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			ft_putstr_fd(tmp->key,STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(tmp->value, STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}


int	ft_exit(t_comm *lst)
{
	// int locate = locate_env_key(lst->e, "SHLVL", 0);
	int shlvl;
	char *tmp;
	exit (0);
}
int check_builtin(t_comm *lst, char **env)
{
	if (ft_strncmp(*lst->command_str, "echo", 5) == 0) ///'-n' should work
	{
		return (ft_echo(lst));
	}
	else if (ft_strncmp(*lst->command_str, "cd", 3) == 0) ///'only a relative or absolute path'
	{
		return (ft_cd(lst));
	}
	else if (ft_strncmp(*lst->command_str, "pwd", 4) == 0)
	{
		return (ft_pwd(lst));
	}
	else if (ft_strncmp(*lst->command_str, "export", 7) == 0)
	{
		//ft_export();
	}
	else if (ft_strncmp(*lst->command_str, "unset", 6) == 0)
	{
		//ft_unset();
	}
	else if (ft_strncmp(*lst->command_str, "env", 4) == 0) /// 'no arguments'
	{
		return (ft_env(lst));
	}
	else if (ft_strncmp(*lst->command_str, "exit", 5) == 0)
	{
		return (ft_exit(lst));
	}
	return (EXIT_FAILURE);
}
