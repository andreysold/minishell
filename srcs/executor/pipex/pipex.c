/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/15 15:09:24 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <crt_externs.h>
#include "pipex.h"

void close_pipes(int *pipes, int count_node)
{
	int i;
	int n;

	i = 0;
	n = 2 * (count_node - 1);
	/*printf("%s!! %d %s\n", RED, n,   RESET);
	fflush(NULL);*/
	while (i < n)
		close(pipes[i++]);
}

void wait_childs(int n)
{
	int	i;
	int	status;

	i = 0;
	while (i < n)
	{
		wait(&status);
		// if (WIFEXITED(status) && status != 0)
		// {
		// 	printf("%sexit status = %d%s\n",RED, WIFEXITED(status), RESET);
		// 	fflush(NULL);
		// }
		i++;
	}
}

static inline int cmd_position(int kind, t_comm *tmp)
{
	if (tmp->count_node > 1 && tmp->next)
		kind = MIDDLE;
	if (kind == MIDDLE && (tmp->count_node > 1 && tmp->next->next == NULL))
		kind = END;
	return (kind);
}

static inline void close_in_out_file(t_comm *tmp)
{
	int err;

	err = 0;
	if (tmp->outfile != -2)
		err = close(tmp->outfile);
	if (tmp->infile != -2)
		err = close(tmp->infile);
	if (err != 0)
		perror("close_in_out_file:");
}

static inline void heredoc(t_comm *tmp)
{
	char	*str;
	int		here_len;

	here_len = (int)ft_strlen(tmp->here);
	while (1)
	{
		str = readline("> ");
		if (ft_strncmp(str, tmp->here, here_len + 1) == 0) ///"tmp.here\0", so+1
		{
			break ;
		}
		ft_putendl_fd(str, tmp->infile);
		free(str);
	}
	free(str);
}

static inline void redirect(t_comm *tmp)
{
	int err;

	err = 0;
	if (tmp->infile != FD_UNUSED)
	{
		if (tmp->here != NULL) /// test cmd < 21 << pop | cmd << pop < 21
		{
			heredoc(tmp);
			close(tmp->infile);
			tmp->infile = open(".tmp", O_RDONLY);
		}
		err = dup2(tmp->infile, STDIN_FILENO);
	}
	if (tmp->outfile != FD_UNUSED)
	{
		err = dup2(tmp->outfile, STDOUT_FILENO);
	}
	if (err != 0)
		perror("redirect:");
}

static inline int *open_pipes(t_comm *tmp)
{
	int i;
	int	*pipes;

	pipes = ft_calloc(sizeof(int), 2 * (tmp->count_node - 1));
	if (!(pipes))
		perror("open_pipes: can't malloc");
	i = 0;
	while (i < (tmp->count_node - 1))
	{
		if (pipe(pipes + 2 * i) == -1)
			perror("open_pipes: can't open pipe");
		i++;
	}
	return (pipes);
}

static inline void pipe_switch(int i, int kind, int *pipes, t_comm *tmp)
{
	if (tmp->count_node == 2)
	{
		//todo second condition always true
		if (kind == START && tmp->next != NULL)
		{
			///    1 =>
			dup2(pipes[1], STDOUT_FILENO);
		}
		else if (kind == END)
		{
			/// => 2
			dup2(pipes[0], STDIN_FILENO);
		}
	}
	else
	{
		//todo second condition always true
		if (kind == START && tmp->next != NULL)
		{
			///    1 =>
			dup2(pipes[2 * i + 1], STDOUT_FILENO); ///1
		}
		else if (kind == MIDDLE)
		{
			/// => 2 =>
			dup2(pipes[2 * i - 2], STDIN_FILENO); ///0
			dup2(pipes[2 * i + 1], STDOUT_FILENO); ///3
		}
		else if (kind == END)
		{
			/// => 3
			dup2(pipes[2 * i - 2], STDIN_FILENO); ///2
		}
	}
}


static inline int check_builtin(t_comm *tmp, char **env)
{
	// ft_putstr_fd("builtin detected -- ", 1);
	// ft_putendl_fd(tmp->command_str[0],1);
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

int pipex(t_comm *lst, char **env)
{
	//for test:  ls -l | head -6 | cut -b 1-10
	// echo p | echo r | echo i | echo v | echo e | echo t
	int		i;
	int		*pipes;
	t_comm	*tmp;
	int		kind;

/*	lst->outfile = FD_UNUSED;
	lst->infile = FD_UNUSED;
	/// open file to write heredoc
	lst->infile = open("tmp", O_CREAT |  O_WRONLY |  O_TRUNC, 0644);
	if (lst->infile == -1)
		perror("Can't open");
	lst->here = ft_strdup("pop");*/

	tmp = lst;
	pipes = open_pipes(tmp);
	kind = START;
	i = 0;
	while (tmp != NULL)
	{
		if (fork() == 0)
		{
			if (check_builtin(tmp, env) == EXIT_SUCCESS)
				exit(EXIT_SUCCESS);
			if (tmp->count_node > 1)
				pipe_switch(i, kind, pipes, tmp);
			if (tmp->infile != FD_UNUSED || tmp->outfile != FD_UNUSED)
				redirect(tmp);
			close_pipes(pipes, tmp->count_node);
			close_in_out_file(tmp);
			if (execve(find_command_path(tmp->command_str[0], env), tmp->command_str, env) == -1)
			{
				errno = 0;
				ft_putstr_fd("e-bash: ", 2);
				ft_putstr_fd(tmp->command_str[0], 2);
				ft_putendl_fd(": command not found", 2);
				return (-1);
			}
		}
		if (tmp->here)
			unlink(".tmp");
		kind = cmd_position(kind, tmp);
		tmp = tmp->next;
		i++;
	}
	tmp = lst;
	close_pipes(pipes, tmp->count_node);
	close_in_out_file(tmp); /// ??? it doesn't close in each node | mb no need
	wait_childs( tmp->count_node);
	return (0);
}
