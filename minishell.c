/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/27 19:56:16 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_all_list(t_comm *head)
{
	int i;

	if (!head)
		return ;
	remove_all_list(head->next);
	if (head->here)
		free(head->here);
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

void	clean_env(char **env, t_comm *lst)
{
	int	i;
	int	len;

	i = 0;
	if (env)
	{
		while (env[i]) //todo mb + 1
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

int	ft_process4(char *str, t_envp *list_env)
{
	t_comm	*lst;
	char	**new_env;

	lst = NULL; ///fixed
	new_env = ft_update_env(list_env);
	lst = ft_parser4(lst, str, list_env);
	if (lst == NULL)
	{
		if (new_env)
			clean_env(new_env, lst);
		remove_all_list(lst);
		return (0);
	}
	if (executor(lst, new_env) == -1)
		return (-1);
	if (new_env)
		clean_env(new_env, lst);
	remove_all_list(lst);
	return (0);
}

void	handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(1,"  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
	g_error_status = 1;
}

void	ft_up_shlvl(t_envp *list_env)
{
	t_envp	*head;
	int		shlvl;
	char	*tmp;
	int		locate;

	locate = 0;
	head = list_env;
	locate = locate_env_key(head, "SHLVL", 0);
	if (locate == -1)
		add_to_env(list_env, "SHLVL", "1", 0);
	else
	{
		shlvl = ft_atoi(get_env_value(list_env, locate, 0)) + 1;
		tmp = ft_itoa(shlvl);
		upd_env_value(list_env, tmp, locate, 0);
		if (tmp)
			free(tmp);
	}
}

int	ft_main_circle(char *str, t_envp *list_env)
{
	int i;
	int len;


	i = 0;
	str = readline("bash:");
	len = ft_strlen(str);
	if (str[0] == ' ')
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (i == len)
			return (0);
	}
	if (str && *str)
	{
		add_history(str);
		if (ft_lexer(str) != -1)
		{
			if (ft_process4(str, list_env) == -1)
				exit(0);
		}
		else
			free (str);
	}
	else if (str == NULL)
	{
		write(1, "exit\n", 5);
		remove_all_env_list(list_env);
		exit(0);
	}
	return (0);
}

int main(int ac, char **av, char **env)
{
	char		*str;
	char		*name;
	char		**envp;
	t_envp		*list_env;

	list_env = NULL;
	list_env = ft_node_env(list_env, env);
	ft_up_shlvl(list_env);
	//if (ac > 1)
	//	printf("Error arg\n");
	//	exit(0);
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		ft_main_circle(str, list_env);
	}
	remove_all_env_list(list_env);
	return (0);
}