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

char	*get_env_value(t_envp *envp, int location) //mb there's no need (*tmp+i)
{
	int		i;
	t_envp	*tmp;

	i = 0;
	tmp = envp;
	while (tmp)
	{
		if (i == location)
			return (tmp->value);
		i++;
		tmp = tmp->next;
	}
	return (NULL);
}

void	upd_env_value(t_envp *envp, char *value, int location)
{
	int		i;
	t_envp	*tmp;

	i = 0;
	tmp = envp;
	while (tmp)
	{

		if (i == location)
		{
			printf("!!! upd val %s\n", tmp->key);
			fflush(NULL);
			break ;
		}
		i++;
		tmp = tmp->next;
	}
	if (tmp)
		tmp->value = ft_strdup(value); //todo do i need strdup?

}

void	add_to_env(t_envp *envp, char *new_key, char *new_value)
{
	t_envp	*node;
	t_envp	*tmp;

	node = malloc(sizeof(t_envp));
	if (node == NULL)
		exit(0);
	node->key = new_key;
	node->value = new_value;
	node->next = NULL;
	tmp = envp;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

///@return -1 if fails
int	locate_env_key(t_envp *envp, char *key)
{
	int		i;
	size_t	key_len;
	t_envp	*tmp;

	i = 0;
	key_len = ft_strlen(key);
	tmp = envp;
	while (tmp)
	{
		if (ft_strncmp(key, tmp->key, key_len) == 0)
			return (i);
		i++;
		tmp = tmp->next;
	}
	return (-1);
}

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
		ft_putchar_fd(' ', 1);
		i++;
	}
	if (n_flag == 0)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}

/**	copy PATH
 *	add new key-value as OLD_PATH
 *	change PATH to new_path
 *	chdir(new_path) //
**/
int	ft_cd(t_comm *tmp)
{
	char	*new_path;
	char	*old_path;
	int		location;
	char	*clean;

	location = locate_env_key(tmp->e, "PWD");
	old_path = get_env_value(tmp->e, location);
	new_path = ft_strjoin(old_path, "/");
	clean = new_path;
	new_path = ft_strjoin(new_path, tmp->command_str[1]);
	free(clean);
	if (location != -1)
		upd_env_value(tmp->e, new_path, location);
//	printf("!!! upd val path %s\n", tmp->value);
//	fflush(NULL);
	location = locate_env_key(tmp->e, "OLDPWD");
	if (location == -1)
		add_to_env(tmp->e, "OLDPWD", old_path);
	else
		upd_env_value(tmp->e, old_path, location);
	printf("!!! oldpwd %s\n", get_env_value(tmp->e, locate_env_key(tmp->e,
																 "OLDPWD")));
	fflush(NULL);
	chdir(new_path);
	free(new_path);
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
//	char	*buf;
	char	*pwd;

//	buf = NULL;
//	pwd = getcwd(buf, 0);
	pwd = get_env_value(tmp->e, locate_env_key(tmp->e, "PWD"));
	ft_putendl_fd(pwd, STDOUT_FILENO);
//	free(buf);
	//free(pwd);
	return (EXIT_SUCCESS);
}

int check_builtin(t_comm *tmp, char **env)
{
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
