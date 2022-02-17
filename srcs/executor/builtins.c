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
	int		i;
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
			home = get_env_value(lst->e, pos, 0);
			*new_path = home;
			pos = locate_env_key(lst->e,"PWD", 0);
			if (pos != -1)
				upd_env_value(lst->e, home, pos, 0);
		}
	}
	else if (ft_strncmp(lst->command_str[1], "", 1) == 0) //FIXME doesn't work
		return (EXIT_FAILURE);
	else
		*new_path = lst->command_str[1];
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
	char	*buf;

	printf("|>%s<|\n", lst->command_str[1]);
	buf = NULL;
	location = locate_env_key(lst->e, "OLDPWD", 0);
	if (location != -1)
		upd_env_value(lst->e, getcwd(buf, 0), location, 0);
	printf("%s - before origa\n", getcwd(buf, 0));
	if (search_path(lst, &new_path))
		return (EXIT_FAILURE);
	chdir(new_path);
	location = locate_env_key(lst->e, "PWD", 0);
	if (location != -1)
		upd_env_value(lst->e, getcwd(buf, 0), location, 0);
	printf("%s - after origa\n", getcwd(buf, 0));
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
	char	*buf;
	int		location;

	buf = NULL;
	pwd = getcwd(buf, 0);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	location = locate_env_key(lst->e, "PWD", 0);
	if (location != -1)
		upd_env_value(lst->e, pwd, location, 0);
	return (EXIT_SUCCESS);
	/*char	*pwd; //alterinative version if something broke use it
	int		location;

	location = locate_env_key(lst->e, "PWD", 1);
	pwd = get_env_value(lst->e, location, 1);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);*/
}

//int check_valid_key(char c)
//{
//	if (ft_isalpha(c) || ft_)
//		return (1);
//}
int check_export_args(t_comm *copy)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (copy->command_str[i])
	{
		while (copy->command_str[i][j])
		{
			if (ft_isalpha(copy->command_str[i][0]) == 0)
			{
				ft_putstr_fd("bash: export: \'", 2);
				ft_putstr_fd(copy->command_str[i], 2);
				ft_putendl_fd("\': not a valid identifier", 2);
				return (EXIT_FAILURE);
			}
			else
			{
				if (ft_isalnum(copy->command_str[i][j]) == 0)
				{
					ft_putstr_fd("bash: export: \'", 2);
					ft_putstr_fd(copy->command_str[i], 2);
					ft_putendl_fd("\': not a valid identifier", 2);
					return (EXIT_FAILURE);
				}
			}
			j++;
		}
		i++;
	}
}

int	ft_export(t_comm *lst) //todo there is no ascii output
{
	t_comm	*copy;
	t_envp	*tmp;
	int		i;
	int		j;

	if (lst->command_str[1] == NULL)
	{
		tmp = lst->e;
		while (tmp)
		{
			if (tmp->key)
			{
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(tmp->key,STDOUT_FILENO);
				if (tmp->value)
				{
					ft_putstr_fd("=\"", STDOUT_FILENO);
					ft_putstr_fd(tmp->value, STDOUT_FILENO);
					ft_putendl_fd("\"", STDOUT_FILENO);
				}
			}
			tmp = tmp->next;
		}
	}
	else
	{
		copy = lst;
		check_export_args(copy);
	}

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
		return (ft_export(lst));
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
		//ft_exit();
	}
	return (EXIT_FAILURE);
}
