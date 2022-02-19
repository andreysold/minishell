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

//	printf("|>%s<|\n", lst->command_str[1]);
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
//	printf("%s - after origa\n", getcwd(buf, 0));
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
int check_export_arg(t_comm *copy, int i, char **key, char **value)
{
	int	j;
	int	value_start;

	j = 0;
	value_start = 0;
	while (copy->command_str[i][j])
	{
		//word should start with letter
		if (j == 0 && ft_isalpha(copy->command_str[i][j]) == 0)
		{
			return (EXIT_FAILURE);
		}
		else if (j != 0)
		{
			if (copy->command_str[i][j] == '=')
			{
				if (copy->command_str[i][j++])
				{
//					j++;
					value_start = j;
					while (copy->command_str[i][j])
						j++;
					*value = ft_substr(copy->command_str[i], value_start, j + 1);
					//fixme free
				}
				else
					break ;

			}
			else if ((copy->command_str[i][j + 1] && copy->command_str[i][j + 1] == '=') || !copy->command_str[i][j + 1])
			{
				*key = ft_substr(copy->command_str[i], 0, j + 1); //fixme free
			}
			else if (ft_isalnum(copy->command_str[i][j]) == 0)
				return (EXIT_FAILURE);
//			else //todo check condition, if alnum fail go to error
//				break ; //feel it could stop all while
		}
		j++;
	}
	return (EXIT_SUCCESS);
}

int	ft_export(t_comm *lst) //todo there is no ascii output
{
	int location;
	char *key;
	char *value;
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
		while (copy->command_str[i])
		{
			key = NULL;
			value = NULL;
			if (check_export_arg(copy, i, &key, &value))
			{
				ft_putstr_fd("bash: export: \'", 2);
				ft_putstr_fd(copy->command_str[i], 2);
				ft_putendl_fd("\': not a valid identifier", 2);
			}
//			printf("<key>-<value> :: <%s>-<%s>\n", key, value);
			if (key)
			{
				location = locate_env_key(copy->e, key, 0);
				//todo check if copy may move
				if (location == -1)
					add_to_env(copy->e, key, value, 0);
				else
					upd_env_value(copy->e, value, location, 0);
				free(key);
				if (value)
					free(value);
			}
			i++;
		}
	}
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
		location = locate_env_key(prev, lst->command_str[1], 0);
		if (location != -1 && len > 0 && location + 1 > len)
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
		return (ft_unset(lst));
	}
	else if (ft_strncmp(*lst->command_str, "env", 4) == 0) /// 'no arguments'
	{
		return (ft_env(lst));
	}
	else if (ft_strncmp(*lst->command_str, "exit", 5) == 0)
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
		ft_putendl_fd(lst->command_str[0], 1);
		return (EXIT_SUCCESS);
	}
	else if (bool == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (-1);
}
