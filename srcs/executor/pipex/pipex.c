/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/22 12:43:37 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <termios.h>

#include <sys/types.h>
#include <unistd.h>

void	handler22(int sig)
{
	if (sig == SIGINT)
	{
		(void) sig;
		write(2, "\n", 1);
		g_error_status = 130;
	}
	if (sig == SIGQUIT)
	{
		(void) sig;
		write(2, "Quit: 3\n", 8);
		g_error_status = 131;
	}
}

void	ft_handler_herdoc(int sig)
{
	(void) sig;
	exit (0);
}

void close_pipes(int *pipes, int count_node)
{
	int i;
	int n;

	i = 0;
	n = 2 * (count_node - 1);
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
		if (WEXITSTATUS(status) && status != 0)
		{
			g_error_status = 127;
			// printf("%sexit status = %d%s\n",RED, WIFEXITED(status), RESET);
			fflush(NULL);
		}
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

void	handler_her(int sig)
{
	// (void)sig;
	if (sig == SIGINT)
	{
		// printf("A\n");
		g_error_status = 1;
		exit (g_error_status);
	}
	// if (sig == SIGQUIT)
	// {

	// }
}

void	ff(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	// rl_on_new_line();
	// rl_redisplay();
	// write(1,"  \b\b\n", 5);
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// rl_redisplay();
	g_error_status = 1;
	// write(1, "a\n", 2);
	exit (g_error_status);
}

static inline void heredoc(t_comm *tmp)
{
	char	*str;
	int		here_len;

	here_len = (int)ft_strlen(tmp->here);
	while (1)
	{
		str = readline("> ");
		if (str == NULL)
			exit (EXIT_SUCCESS);
		else if (ft_strncmp(str, tmp->here, here_len + 1) == 0)
			exit (EXIT_SUCCESS);
		ft_putendl_fd(str, tmp->infile);
		free(str);
	}
}

static inline void redirect(t_comm *tmp)
{
	int err;

	err = 0;
	if (tmp->infile != FD_UNUSED)
	{
		// if (tmp->here != NULL) /// test cmd < 21 << pop | cmd << pop < 21
		// {
		// 	heredoc(tmp);
		// 	close(tmp->infile);
		// 	tmp->infile = open(".tmp", O_RDONLY);
		// }
		err = dup2(tmp->infile, STDIN_FILENO);
	}
	if (err != 0)
		perror("1redirect:");
	err = 0;
	if (tmp->outfile != FD_UNUSED)
		err = dup2(tmp->outfile, STDOUT_FILENO);
	if (err != 0)
		perror("2redirect:");
}

static inline int *open_pipes(t_comm *tmp)
{
	int i;
	int	*pipes;

	if (tmp->count_node > 1)
	{
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
			dup2(pipes[1], STDOUT_FILENO); ///1
		}
		else if (kind == MIDDLE)
		{
			/// => 2 =>
			dup2(pipes[0], STDIN_FILENO); ///0
			dup2(pipes[3], STDOUT_FILENO); ///3
		}
		else if (kind == END)
		{
			/// => 3
			dup2(pipes[2], STDIN_FILENO); ///2
		}
	}
}

void	bash_error(char *first_part, char *cmd, char *last_part)
{
	ft_putstr_fd(first_part, 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(last_part, 2);
}

void	handler2(int sig)
{
	if (sig == SIGINT)
	{
		(void) sig;
		write(2, "\n", 1);
		g_error_status = 130;
		// exit (0);
	}
	if (sig == SIGQUIT)
	{
		(void) sig;
		write(2, "Quit: 3\n", 8);
		g_error_status = 131;
		// exit (0);
	}
}

int pipex(t_comm *lst, char **env)
{
	//for test:  ls -l | head -6 | cut -b 1-10
	// echo p | echo r | echo i | echo v | echo e | echo t
	int		i;
	int		*pipes;
	t_comm	*tmp;
	int		kind;
	pid_t 	pid;
	int bool;
	int status;

	tmp = lst;
	pipes = open_pipes(tmp);
	kind = START;
	i = 0;
	while (tmp != NULL && tmp->cmd)
	{
		if (tmp->count_node == 1)
		{
			bool = builtins(tmp, env);
			if (bool != -1)
				return (bool);
			if (tmp->here != NULL)
			{
				pid = fork();
				if (pid != 0)
				{
					signal(SIGINT, SIG_IGN);
//					signal(SIGQUIT)
				}
				if (pid == 0) /// test cmd < 21 << pop | cmd << pop < 21
				{
					signal(SIGINT, ff);
					signal(SIGQUIT, SIG_IGN);
					heredoc(tmp);
				}
				wait(&status);
				if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
				{
					g_error_status = 1;
					return (g_error_status);
				}
				tmp->infile = open(".tmp", O_RDONLY);
			}
		}
		signal(SIGINT, handler22);
		signal(SIGQUIT, handler22);
		if (fork() == 0)
		{
			if (tmp->count_node > 1)
				pipe_switch(i, kind, pipes, tmp);
			if (tmp->infile != FD_UNUSED || tmp->outfile != FD_UNUSED)
				redirect(tmp);
			close_pipes(pipes, tmp->count_node);
			close_in_out_file(tmp);
			bool = builtins(tmp, env);
			if (bool != -1)
				exit (bool);
			if (execve(find_command_path(tmp->cmd[0], env),
					   tmp->cmd, env) == -1)
			{
				bash_error("bash: ", tmp->cmd[0], ": command not found");
				exit(EXIT_FAILURE);
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
	wait_childs(tmp->count_node);
	if (lst->count_node > 1)
		free(pipes);
	return (EXIT_SUCCESS);
}
