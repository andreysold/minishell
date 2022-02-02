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
	if (fd[2] == -1)
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
	}
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
	int		fd[3];
	pid_t	pid[2];

	errno = 0;
//	if (lst->count_word < 1)
//		error_n_exit("You give 0 commands");
	printf("%s --- \n", lst->next->command_str[0]);
/*	///added (to 5) 2 argc for infile/outfile trigger; now ac = 7
*//*	if (argc > 7)
		error_n_exit("You should give four arguments");*//*
	ft_memset((void*)fd, 0, sizeof(fd[3]));
	if (pipe(fd) == -1)
		error_n_exit("Can't create a pipe");

	pid[0] = fork();
	if (pid[0] < 0)
		error_n_exit("Can't fork a new process");
	if (pid[0] == 0)
	{
		///infile will use file in argv[1]. if u have trigger in argv[5]
		if (argv[6][0] == '<')
			fd[2] = open(argv[1], O_RDONLY);
		else
			fd[2] = 0;
		process(pid, fd, argv[2], env);
	}
	pid[1] = fork();
	if (pid[1] < 0)
		error_n_exit("Can't fork a new process");
	if (pid[1] == 0)
	{
		///this is for outfile
		if (argv[5][0] == '>')
			fd[2] = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
		process(pid, fd, argv[3], env);
	}
	close_fd_and_waitpid(fd, pid);*/
	return (0);
}

/*int main(int argc, char **argv, char **env)
{
	pipex(argc, argv, env);
	return (0);
}*/