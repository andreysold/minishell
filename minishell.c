/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/20 20:46:12 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_all_list(t_comm *head)
{
	int i;

	if (!head)
		return ;
	remove_all_list(head->next);
	if (head->cmd)
	 	{
			i = 0;
			while (head->cmd[i])
				i++;
			leak_case(i, head->cmd);
		}
	free(head);
}

void	remove_all_env_list(t_envp *head)
{
	if (!head)
		return ;
	remove_all_env_list(head->next);
	if (head->key)
		free(head->key);
	if (head->value)
		free(head->value);
	free(head);
}

void	ft_free_list(t_comm *lst)
{
	int		i;
	t_comm	*head;
	t_envp	*head2;

	// while (lst->e)
	// {
	// 	// printf("")
	// 	head2 = lst->e;
	// 	if (lst->e->key)
	// 		free (lst->e->key);
	// 	if (lst->e->value)
	// 		free (lst->e->value);
	// 	lst->e = lst->e->next;
	// 	free (head2);
	// }
	while (lst)
	{
		head = lst;
		if (lst->cmd)
		{
			i = 0;
			while (lst->cmd[i])
				i++;
			ft_no_malloc(lst->cmd);
		}
		lst = lst->next;
		free (head);
	}
}

char	*ft_get_key(int count, char **env)
{
	char	*str;
	int		i;
	int		z;

	i = 0;
	while (env[i])
	{
		if (i == count)
		{
			z = 0;
			while (env[i][z] && env[i][z] != '=')
				z++;
			str = ft_substr(env[i], 0, z);
			break ;
		}
		i++;
	}
	return (str);
}

char	*ft_get_value(int count, char **env)
{
	char	*str;
	int		i;
	int		z;
	char	*clean;

	i = 0;
	while (env[i])
	{
		if (i == count)
		{
			z = 0;
			while (env[i][z] && env[i][z] != '=')
				z++;
			str = ft_substr(env[i], z + 1, ft_strlen(env[i]) - z);
			break ;
		}
		i++;
	}
	return (str);
}

t_envp	*ft_node_env(t_envp *e, char **env)
{
	int		count;
	t_envp	*tmp;

	count = 0;
	while (env[count])
		count++;
	e = NULL;
	while (count-- > 0)
	{
		tmp = malloc(sizeof(t_envp));
		if (!tmp)
			return (NULL);
		tmp->key = ft_get_key(count, env);
		tmp->value = ft_get_value(count, env);
		// tmp->key_orig = ft_get_key(count, env);
		// tmp->value_orig = ft_get_value(count, env);
		tmp->count = count;
		tmp->next = e;
		e = tmp;
	}
	return (e);
}

int	ft_cnode(t_envp *l)
{
	int	i;
	t_envp	*tmp;

	i = 0;
	tmp = l;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**ft_update_env(t_envp *l_envp)
{
	int		i;
	t_envp	*tmp;
	char	*clean;
	char	**new_env;
	int count;

	count = ft_cnode(l_envp);
	l_envp->count = count;
	tmp = l_envp;
	new_env = ft_calloc(sizeof(char *), count + 1);
	if (!new_env)
		return (NULL);
	i = 0;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			new_env[i] = ft_strdup(tmp->key);
			clean = new_env[i];
			new_env[i] = ft_strjoin(new_env[i], "=");
			free(clean);
			// clean = NULL;
			clean = new_env[i]; ///
			new_env[i] = ft_strjoin(new_env[i], tmp->value);
//			if (clean)
			 //fixme how
		}
		if (clean)
			free(clean);
//		if (tmp->value)
//			new_env[i] = ft_strjoin(new_env[i], tmp->value);
//		printf("|%s| |%s|\n", tmp->key, tmp->value);
		tmp = tmp->next;
		i++;
	}
	// new_env[i] = NULL;
	return (new_env);
}

void	clean_env(char **env, t_comm *lst)
{
	int	i;
	int	len;

	i = 0;
	if (env)
	{
		while (i < lst->e->count) //todo mb + 1
		{
			free(env[i]);
			i++;
		}
		// free(env[i]);
		free(env);
	}
}

void	ft_no_malloc(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free (str[i]);
		i++;
	}
	free (str);
}

char	**ft_get_envp(char **env)
{
	int		i;
	char	**envp;

	i = 0;
	while (env[i])
		i++;
	envp = (char **)malloc(sizeof(char *) * i + 1);
	if (!envp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		envp[i] = ft_substr(env[i], 0, ft_strlen(env[i]));
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	ft_check_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '>' && !(ft_isdigit(str[i + 1])))
		{
			i += 2;
			if (str[i] == '>')
				return (-1);
		}
		i++;
	}
	return (1);
}

int	ft_process4(char *str, t_envp *envp)
{
	t_comm	*lst;
	char	**env;

	lst = NULL; ///fixed
	env = ft_update_env(envp);
	// lst = malloc(sizeof(t_comm));
	// if (!lst)
	// 	return (-1);
	// ft_memset((void *)lst, 0, sizeof(t_comm));
	lst = ft_parser4(lst, str, envp);
/*	if (executor(lst, env) == -1)
		return (1);*/
	executor(lst, env);
	if (env)
		clean_env(env, lst);
	remove_all_list(lst);
	//env = ft_update_env(envp); /////
	// ft_free_list(lst);
	return (0);
}

int	main(void)
{
	char		*str;
	char		*name;
	t_envp		*l_envp;
	extern char	**environ;

	// l_envp = malloc(sizeof(t_envp *));
	// if (!l_envp)
	// 	return (-1);
	l_envp = ft_node_env(l_envp, environ); // key value 

	while (1)
	{
		str = readline("bash:");
		if (str && *str)
		{
			add_history(str);
			// if (ft_lexer(str) != -1)
			// {
			if (ft_process4(str, l_envp) == -1)
				exit(0);
			// }
			// else
				// free (str);

		}
		else if (str == NULL)
			exit (0);
	}
	remove_all_env_list(l_envp);
}
