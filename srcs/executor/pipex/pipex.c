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
#include <math.h>

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

void close_pipes(int *pipes, int n)
{
	int i;

	i = 0;
	while (i < n)
		close(pipes[i++]);
}

void wait_childs(int status, int n)
{
	int i;

	i = 0;
	while (i++ < n)
		wait(&status);
}

int pipex_alt(t_comm *lst, char **env)
{
	//for test:  ls -l | head -6 | cut -b 1-10
	int		status;
	int		i;
	int		pipes[4];
	t_comm	*tmp;
	int		kind;

	// arguments for commands; your parser would be responsible for
	// setting up arrays like these

	/*char *cat_args[] = {"ls", "-l", NULL};
	char *grep_args[] = {"head", "-6", NULL};
	char *cut_args[] = {"cut", "-b", "1-10", NULL};
*/
	// make 2 pipes (cat to grep and grep to cut); each has 2 fds

	tmp = lst;
	ft_memset((void *)pipes, 0, sizeof(pipes));
//	printf("%d -- \n", lst->count_node);
	if (pipe(pipes) == -1) // sets up 1st pipe
		error_n_exit("Can't create a pipe");
	if (pipe(pipes + 2) == -1) // sets up 2st pipe
		error_n_exit("Can't create a pipe");

	// we now have 4 fds:
	// pipes[0] = read end of ls->head pipe (read by grep)
	// pipes[1] = write end of ls->head pipe (written by cat)
	// pipes[2] = read end of head->cut pipe (read by cut)
	// pipes[3] = write end of head->cut pipe (written by grep)

	// Note that the code in each if is basically identical, so you
	// could set up a loop to handle it.  The differences are in the
	// indicies into pipes used for the dup2 system call
	// and that the 1st and last only deal with the end of one pipe.

	// fork the first child (to execute ls)
	kind = START;
	while (tmp != NULL)
	{
//		printf("%d -- kind\n", kind);
//		fflush(NULL);
		//write(2, "@$\n", 3);
		if (fork() == 0)
		{
//			write(2, &kind, 1);
//			write(1, "\n", 1);
//			printf("%d -- kind\n", kind);
//			fflush(NULL);
			if (tmp->count_node > 1)
			{
				if (tmp->count_node == 2)
				{
					if (kind == START && tmp->next->command_str != NULL)
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
					if (kind == START && tmp->next->command_str != NULL)
					{
						///    1 =>
						dup2(pipes[1], STDOUT_FILENO);
					}
					else if (kind == MIDDLE)
					{
						/// => 2 =>
						dup2(pipes[0], STDIN_FILENO);
						dup2(pipes[3], STDOUT_FILENO);
					} else if (kind == END)
					{
						/// => 3
						dup2(pipes[2], STDIN_FILENO);
					}
				}
			}
//			printf("%d -- kind\n", kind);
//			fflush(NULL);
			close_pipes(pipes, tmp->count_node);
//			printf("%d -- kind\n", kind);
//			fflush(NULL);
			execve(find_command_path(tmp->command_str[0], env), \
			tmp->command_str, env);
		}
//		printf("%d -- kind\n", kind);
//		fflush(NULL);
		if (kind == END)
			break ;
		if (tmp->count_node > 1 && tmp->next->command_str)
		{
			kind = MIDDLE;
		}
		if (tmp->count_node > 1 && tmp->next->next->command_str == NULL)
		{
			kind = END;
		}
//		printf("%d -- kind\n", kind);
//		fflush(NULL);
		//else
//		if (tmp->count_node > 1)
			tmp = tmp->next;
	}
//	printf("%d -- kind\n", kind);
//	fflush(NULL);
/*
	if (fork() == 0)
	{
		// replace ls's stdout with write part of 1st pipe
		dup2(pipes[1], STDOUT_FILENO);

		// close all pipes (very important!); end we're using was safely copied

		i = 0;
		while (i < 4)
			close(pipes[i++]);

		execve(find_command_path(lst->command_str[0], env), lst->command_str, env);
	}
	else
	{
		// fork second child (to execute head)

		if (fork() == 0)
		{
			// replace head's stdin with read end of 1st pipe

			dup2(pipes[0], STDIN_FILENO);

			// replace head's stdout with write end of 2nd pipe

			dup2(pipes[3], STDOUT_FILENO);

			// close all ends of pipes

			i = 0;
			while (i < 4)
				close(pipes[i++]);

			execve(find_command_path(lst->next->command_str[0], env),
				   lst->next->command_str, env);
//			execvp(*grep_args, grep_args);
		}
		else
		{
			// fork third child (to execute cut)

			if (fork() == 0)
			{
				// replace cut's stdin with input read of 2nd pipe
				dup2(pipes[2], STDIN_FILENO);

				// close all ends of pipes
				i = 0;
				while (i < 4)
					close(pipes[i++]);

				execve(find_command_path(lst->next->next->command_str[0], env),
					   lst->next->next->command_str, env);
//				execvp(*cut_args, cut_args);
			}
		}
	}
*/

	// only the parent gets here and waits for 3 children to finish
	tmp = lst;
	close_pipes(pipes, tmp->count_node);

	/*for (i = 0; i < 3; i++)
		wait(&status);*/
	wait_childs(status, tmp->count_node);
	return (0);
}

/*int main(int argc, char **argv, char **env)
{
	pipex(argc, argv, env);
	return (0);
}*/
