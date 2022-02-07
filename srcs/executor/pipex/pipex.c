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

void close_pipes(int *pipes, int pipes_amount)
{
	int i;
	int n;

	i = 0;
	n = 2 * pipes_amount;
	/*printf("%s!! %d %s\n", RED, n,   RESET);
	fflush(NULL);*/
	while (i < n)
		close(pipes[i++]);
}

void wait_childs(int status, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
/*		printf("%sPID %d | cmd have %d status %s\n", GREEN, getpid(),
			   status,   RESET);
		fflush(NULL);*/
		wait(&status);
//		waitpid(NULL, &status, 0);
		if (WIFEXITED(status))
		{
			printf("%sexit status = %d%s\n",RED, WIFEXITED(status), RESET);
			fflush(NULL);
		}
		i++;
//		printf("%s%d process exited%s\n", RED, i, RESET);
//		fflush(NULL);
	}
}

int pipex_alt(t_comm *lst, char **env)
{
	//for test:  ls -l | head -6 | cut -b 1-10
	// echo p | echo r | echo i | echo v | echo e | echo t
	int		status = 0;
	int		i;
	int		*pipes;
	t_comm	*tmp;
	int		kind;

	tmp = lst;
	lst->outfile = -2;
	lst->infile = -2;
	pipes = ft_calloc(sizeof(int), 2 * (lst->count_node - 1));
	if (!pipes)
		return (EXIT_FAILURE);
	lst->outfile = open("ebashu.txt", O_CREAT |  O_WRONLY |  O_TRUNC, 0644);
	if (lst->outfile == -1)
		perror("Can't open");
/*	i = 0;
	while (i < (lst->count_node - 1))
		pipes[i++] = 0;*/
	kind = START;
	i = 0;
	while (i < (lst->count_node - 1))
	{
		if (pipe(pipes + 2 * i) == -1)
			error_n_exit("Can't create a pipe");
		i++;
	}
	i = 0;
	while (tmp != NULL)
	{
		if (fork() == 0)
		{
			if (tmp->count_node > 1)
			{
				if (tmp->count_node == 2)
				{
					//todo second condition always true
					if (kind == START && tmp->next != NULL)
					{
						///    1 =>
						dup2(pipes[1], STDOUT_FILENO);
						if (lst->infile != -2)
						{
							dup2(lst->infile, STDIN_FILENO);
						}
					}
					else if (kind == END)
					{
						/// => 2
						dup2(pipes[0], STDIN_FILENO);
						if (lst->outfile != -2)
						{
							dup2(lst->outfile, STDOUT_FILENO);
						}
					}
				}
				else
				{
					//todo second condition always true
					if (kind == START && tmp->next != NULL)
					{
						///    1 =>
						dup2(pipes[2 * i + 1], STDOUT_FILENO); ///1
						if (lst->infile != -2)
						{
							dup2(lst->infile, STDIN_FILENO);
						}
					}
					else if (kind == MIDDLE)
					{
						/// => 2 =>
						dup2(pipes[2 * i - 2], STDIN_FILENO); ///0
						dup2(pipes[2 * i + 1], STDOUT_FILENO); ///3
					} else if (kind == END)
					{
						/// => 3
						dup2(pipes[2 * i - 2], STDIN_FILENO); ///2
						if (lst->outfile != -2)
						{
							dup2(lst->outfile, STDOUT_FILENO);
						}
					}
				}
			}
			else
			{
				if (lst->infile != -2)
				{
					dup2(lst->infile, STDIN_FILENO);
				}
				if (lst->outfile != -2)
				{
					dup2(lst->outfile, STDOUT_FILENO);
				}
			}


			close_pipes(pipes, tmp->count_node - 1);

/*			/// getpid last process
			if (kind == END)
			{
				printf("%sPID %d | go to execve%s\n", YELLOW, getpid(), RESET);
				fflush(NULL);
			}*/
			if (execve(find_command_path(tmp->command_str[0], env), \
			tmp->command_str, env) == -1)
			{
				perror("Fail exec!!!!!!!");
				exit(0);
			}
		}

		if (tmp->count_node > 1 && tmp->next)
		{
			kind = MIDDLE;
		}
		if (kind == MIDDLE && (tmp->count_node > 1 && tmp->next->next == NULL))
		{
			kind = END;
		}
		i++;
		tmp = tmp->next;
	}
	tmp = lst;
	close_pipes(pipes, tmp->count_node - 1);
	wait_childs(status, tmp->count_node);
	return (0);
}
