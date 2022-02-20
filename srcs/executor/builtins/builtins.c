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
	while (lst->cmd[i] && !ft_strncmp(lst->cmd[i], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (lst->cmd[i])
	{
		ft_putstr_fd(lst->cmd[i], 1);
		ft_putchar_fd(' ', 1);
		i++;
	}
	if (n_flag == 0)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}

/*static inline void	upd_oldpwd(const t_comm *lst)
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
}*/

static inline int	search_path(const t_comm *lst, char	**new_path)
{
	int		i;
	int		pos;
	char	*home;

	if (lst->cmd[1] == NULL) //todo check is there NULL
	{
		pos = locate_env_key(lst->e,"HOME", 0);
		if (pos == -1)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(lst->cmd[0], 2);
			ft_putendl_fd(": HOME not set", 2);
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
	else if (ft_strncmp(lst->cmd[1], "", 1) == 0) //FIXME doesn't work
		return (EXIT_FAILURE);
	else
		*new_path = lst->cmd[1];
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

//	printf("|>%s<|\n", lst->cmd[1]);
	buf = NULL;
	location = locate_env_key(lst->e, "OLDPWD", 0);
	if (location != -1)
		upd_env_value(lst->e, getcwd(buf, 0), location, 0);
//	printf("%s - before origa\n", getcwd(buf, 0));
	if (search_path(lst, &new_path))
		return (EXIT_FAILURE);
	chdir(new_path);
	location = locate_env_key(lst->e, "PWD", 0);
	if (location != -1)
		upd_env_value(lst->e, getcwd(buf, 0), location, 0);
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
}

size_t	ft_listlen(t_envp *head)
{
	size_t	len;
	t_envp	*buf;

	len = 0;
	buf = head;
	while (buf)
	{
		buf = buf->next;
		len++;
	}
	return (len);
}

int	ft_unset(t_comm *lst)
{
	int		i;
	int		j;
	int		len;
	int		location;
	t_envp	*prev;
	t_envp	*del;

	prev = lst->e;
	if (prev)
	{
		j = 1;
		len = (int)ft_listlen(prev);
		printf("%s len - %d\n", RED, len);
		location = locate_env_key(prev, lst->cmd[1], 0);
		if (location == -1 || len <= 0 || location + 1 > len)
			return (EXIT_FAILURE);
		printf("%s locat - %d\n", GREEN, location);
		while (j <= location - 1)
		{
			prev = prev->next;
			j++;
		}
		printf("%s j - %d\n", YELLOW, location);
		del = prev;
		del = del->next;
		prev->next = del->next;
		printf("%s j - %s%s\n", BLUE, del->key, RESET);
		free(del);
//		prev->next = NULL;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
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
	if (ft_strncmp(*lst->cmd, "echo", 5) == 0) ///'-n' should work
	{
		return (ft_echo(lst));
	}
	else if (ft_strncmp(*lst->cmd, "cd", 3) == 0) ///'only a relative or absolute path'
	{
		return (ft_cd(lst));
	}
	else if (ft_strncmp(*lst->cmd, "pwd", 4) == 0)
	{
		return (ft_pwd(lst));
	}
	else if (ft_strncmp(*lst->cmd, "export", 7) == 0)
	{
		return (ft_export(lst));
	}
	else if (ft_strncmp(*lst->cmd, "unset", 6) == 0)
	{
		return (ft_unset(lst));
	}
	else if (ft_strncmp(*lst->cmd, "env", 4) == 0) /// 'no arguments'
	{
		return (ft_env(lst));
	}
	else if (ft_strncmp(*lst->cmd, "exit", 5) == 0)
	{
		//ft_exit();
	}
	return (-1);
}

int builtins(t_comm *lst, char **env)
{
	int	bool;

	bool = check_builtin(lst, env);
	if (bool == EXIT_SUCCESS)
	{
		ft_putstr_fd("1 check_builtin detected -- ", 1);
		ft_putendl_fd(lst->cmd[0], 1);
		return (EXIT_SUCCESS);
	}
	else if (bool == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (-1);
}
