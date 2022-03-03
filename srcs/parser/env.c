/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 14:05:38 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_get_envp(char **env)
{
	char	**envp;
	int		i;

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

char	**ft_update_env(t_envp *list_env)
{
	int		i;
	char	**massiv;
	int		count;
	t_envp	*tmp;
	char	*clean;

	i = 0;
	tmp = list_env;
	count = ft_cnode(list_env);
	massiv = ft_calloc(sizeof(char *), count + 1);
	if (!massiv)
		return (NULL);
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			ft_get_key_value(massiv, tmp, clean, &i);
			i++;
		}
		tmp = tmp->next;
	}
	massiv[i] = NULL;
	return (massiv);
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
	int		c;

	count = 0;
	while (env[count])
		count++;
	c = count;
	e = NULL;
	while (count-- > 0)
	{
		tmp = malloc(sizeof(t_envp));
		if (!tmp)
			return (NULL);
		tmp->key = ft_get_key(count, env);
		tmp->value = ft_get_value(count, env);
		tmp->count = c;
		tmp->next = e;
		e = tmp;
	}
	return (e);
}
