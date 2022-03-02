/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/27 19:53:44 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	one_cmd_preprocess(t_comm **tmp, char **env)
{
	int		bool;

	if ((*tmp)->count_node == 1 && (*tmp)->cmd[0] != NULL)
	{
		bool = builtins(tmp, env);
		if (bool != -1)
			return (bool);
		if ((*tmp)->here != NULL)
			heredoc(tmp);
	}
	return (-1);
}

static inline void	pipex_routine2(t_comm **tmp, char **env)
{
	int	bool;

	if ((*tmp)->count_node != 1 && (*tmp)->cmd[0] != NULL)
	{
		bool = builtins(tmp, env); //fixme
		if (bool != -1)
			exit (bool);
	}
	if (execve(find_command_path((*tmp)->cmd[0], env), (*tmp)->cmd, env) == -1)
	{
		bash_error("bash: ", (*tmp)->cmd[0], ": command not found");
		exit(g_error_status);
	}
}

static inline void	pipex_routine1(t_comm **tmp, int *pipes, int kind, int i)
{
	//fixme if pipes won't work (add int **PIPES)
	if ((*tmp)->cmd[0] != NULL)
	{
		if ((*tmp)->count_node > 1)
			pipe_switch(i, kind, pipes, (*tmp));
		if ((*tmp)->infile != FD_UNUSED || (*tmp)->outfile != FD_UNUSED)
			redirect((*tmp));
		close_pipes(pipes, (*tmp)->count_node);
		close_in_out_file((*tmp));
	}
}

static inline void	pipex_end(t_comm **lst, int *pipes)
{
	t_comm	*tmp;

	tmp = *lst;
	close_pipes(pipes, tmp->count_node);
	close_in_out_file(tmp); /// ??? it doesn't close in each node | mb no need
	wait_childs(tmp->count_node);
	if ((*lst)->count_node > 1)
		free(pipes);
}

/// arr[0] – i \n arr[1] – kind \n arr[2] - bool
///for test:  ls -l | head -6 | cut -b 1-10
/// echo p | echo r | echo i | echo v | echo e | echo t
int	executor(t_comm **lst, char **env)
{
	int		arr[3];
	int		*pipes;
	t_comm	*tmp;

	tmp = *lst;
	pipes = open_pipes(tmp);
	ft_memset((void *)arr, 0, sizeof(int *));
	while (tmp != NULL)
	{
		arr[2] = one_cmd_preprocess(&tmp, env);
		if (arr[2] != -1)
			return (arr[2]);
		signal(SIGINT, handler22);
		signal(SIGQUIT, handler22);
		if (fork() == 0)
		{
			pipex_routine1(&tmp, pipes, arr[1], arr[0]);
			pipex_routine2(&tmp, env);
		}
		pipex_iterators(&tmp, &arr[1], &arr[0]);
	}
	pipex_end(lst, pipes);
	return (EXIT_SUCCESS);
}
