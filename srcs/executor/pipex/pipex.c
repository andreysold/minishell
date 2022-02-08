/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2021/11/01 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static inline void	close_fd_and_waitpid(int fd[3], int pid[2])
{
	close(fd[0]);
	close(fd[1]);
	close(fd[2]);
	if (pid != NULL)
	{
		waitpid(pid[0], NULL, 0);
		waitpid(pid[1], NULL, 0);
	}
}

static inline void	process(int *pid, int *fd, char *cmd_argv, char **env)
{
/*	///redirects here
 * if (fd[2] == -1)
	{
		if (pid[0] == 0)
			error_n_exit("Can't open file to read");
		if (pid[1] == 0)
			error_n_exit("Can't open file to write");
	}
	else
	{
		if (fd[2] != 0)
		{
			if (pid[0] == 0)
				dup2(fd[2], STDIN_FILENO);
			if (pid[1] == 0)
				dup2(fd[2], STDOUT_FILENO);
		}
	}*/
	if (pid[0] == 0)
		dup2(fd[1], STDOUT_FILENO);
	if (pid[1] == 0)
		dup2(fd[0], STDIN_FILENO);
	close_fd_and_waitpid(fd, NULL);
	execve(find_command_path(cmd_argv, env), ft_split(cmd_argv, ' '), env);
	if (errno != 0)
		error_n_exit("Error");
}

/*
///@param argv[1] –– infile. File can be used for cmd1. It will work only if
/// you use redirect trigger in argv[6]
///@param argv[2] –– cmd1. Imagine next is pipe, next to cmd1
///@param argv[3] –– cmd2.
///@param argv[4] –– outfile. u should use redirect in argv[5]
///@param argv[5] –– necessary for outfile. use >
///@param argv[6] –– necessary for infile. use <
int	pipex(t_comm *lst, char **env)
{
	///@param fd[0] fd[1] -- for pipe i/o
	///@param fd[2] -- using for infile/outfile(open fd)
	int		fd[3];
	pid_t	pid[2];

	errno = 0;
//	if (lst->command_str[0] == NULL)
//		error_n_exit("You give 0 commands");
//	printf("%s --- size -- %lu\n", lst->next->command_str[0], sizeof(pid));
//	///added (to 5) 2 argc for infile/outfile trigger; now ac = 7
//	if (argc > 7)
//		error_n_exit("You should give four arguments");
	ft_memset((void*)fd, 0, sizeof(fd));
	ft_memset((void*)pid, 0, sizeof(pid));
	if (pipe(fd) == -1)
		error_n_exit("Can't create a pipe");

	pid[0] = fork();
	if (pid[0] < 0)
		error_n_exit("Can't fork a new process");
	if (pid[0] == 0)
	{
//		///infile will use file in argv[1]. if u have trigger in argv[5]
//		if (argv[6][0] == '<')
//			fd[2] = open(argv[1], O_RDONLY);
//		else
//			fd[2] = 0;
		process(pid, fd, lst->command_str[0], env);
	}
	pid[1] = fork();
	if (pid[1] < 0)
		error_n_exit("Can't fork a new process");
	if (pid[1] == 0)
	{
//		///this is for outfile
//		if (argv[5][0] == '>')
//			fd[2] = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
		process(pid, fd, lst->next->command_str[0], env);
	}
	close_fd_and_waitpid(fd, pid);
	return (0);
}
*/

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
		if (WIFEXITED(status))
		{
			printf("%sexit status = %d%s\n",RED, WIFEXITED(status), RESET);
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
			tmp->infile = open("tmp", O_RDONLY);
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
//			if (lst->infile != -2)
//			{
//				dup2(lst->infile, STDIN_FILENO);
//			}
		}
		else if (kind == END)
		{
			/// => 2
			dup2(pipes[0], STDIN_FILENO);
//			if (lst->outfile != -2)
//			{
//				dup2(lst->outfile, STDOUT_FILENO);
//			}
		}
	}
	else
	{
		//todo second condition always true
		if (kind == START && tmp->next != NULL)
		{
			///    1 =>
			dup2(pipes[2 * i + 1], STDOUT_FILENO); ///1
//			if (lst->infile != -2)
//			{
//				dup2(lst->infile, STDIN_FILENO);
//			}
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
//			if (lst->outfile != -2)
//			{
//				dup2(lst->outfile, STDOUT_FILENO);
//			}
		}
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

	lst->outfile = FD_UNUSED;
	lst->infile = FD_UNUSED;
	/// open file to write heredoc
	lst->infile = open("tmp", O_CREAT |  O_WRONLY |  O_TRUNC, 0644);
	if (lst->infile == -1)
		perror("Can't open");
	lst->here = ft_strdup("pop");

	tmp = lst;
	pipes = open_pipes(tmp);
	kind = START;
	i = 0;
	while (tmp != NULL)
	{
		if (fork() == 0)
		{
			if (tmp->count_node > 1)
				pipe_switch(i, kind, pipes, tmp);
			if (tmp->infile != FD_UNUSED || tmp->outfile != FD_UNUSED)
				redirect(tmp);
			close_pipes(pipes, tmp->count_node);
			close_in_out_file(tmp);
			if (execve(find_command_path(tmp->command_str[0], env), \
			tmp->command_str, env) == -1)
			{
				perror("Fail exec!!!!!!!");
				exit(0);
			}
		}
		kind = cmd_position(kind, tmp);
		tmp = tmp->next;
		i++;
	}
	tmp = lst;
	close_pipes(pipes, tmp->count_node);
	close_in_out_file(tmp); /// ??? it doesn't close in each node | mb no need
	wait_childs( tmp->count_node);
//	unlink("tmp");
	return (0);
}
