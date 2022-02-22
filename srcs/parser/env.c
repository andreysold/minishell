#include "../../includes/minishell.h"

char **ft_get_envp(char **env)
{
	char **envp;
	int i = 0;

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

char **ft_update_env(t_envp *list_env)
{
	int i;
	char **massiv;
	int count;
	char *tmp1;
	t_envp *tmp;

	tmp = list_env;
	i = 0;
	count = 0;
	count = ft_cnode(list_env);
	massiv = (char **)malloc(sizeof(char *) * count + 1);
	if (!massiv)
		return (NULL);
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			tmp1 = ft_strjoin(tmp->key, "=");
			massiv[i] = ft_strjoin(tmp1, tmp->value);
			free (tmp1);
		}
		i++;
		tmp = tmp->next;
	}
	massiv[i] = NULL;
	return (massiv);
}

char *ft_get_key(int count, char **env)
{
	char *str;
	int i;

	i = 0;
	while (env[i])
	{
		if (i == count)
		{
			int z = 0;
			while (env[i][z] && env[i][z] != '=')
				z++;
			str = ft_substr(env[i], 0, z);
			break ;
		}
		i++;
	}
	return (str);
}

char *ft_get_value(int count, char **env)
{
	char *str;
	int i;

	i = 0;
	while (env[i])
	{
		if (i == count)
		{
			int z = 0;
			while (env[i][z] && env[i][z] != '=')
				z++;
			str = ft_substr(env[i], z + 1, ft_strlen(env[i]) - z);
			break ;
		}
		i++;
	}
	return (str);
}

t_envp *ft_node_env(t_envp *e, char **env)
{
	int c;
	int count;
	t_envp *tmp;

	count = 0;
	while (env[count])
		count++;
	e = NULL;
	c = count;
	while (count-- > 0)
	{
		tmp = malloc(sizeof(t_envp));
		if (!tmp)
			return (NULL);
		tmp->key = ft_get_key(count, env);
		tmp->value = ft_get_value(count, env);
		tmp->count = c;
		// tmp->key_orig = ft_get_key(count, env);
		// tmp->value_orig = ft_get_value(count, env);
		tmp->next = e;
		e = tmp;
	}
	return (e);
}


int	ft_cnode(t_envp *list_env)
{
	int i;
	t_envp *tmp;

	tmp = list_env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return(i);
}