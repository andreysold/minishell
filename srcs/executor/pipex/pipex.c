/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/27 19:55:44 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

#include <unistd.h>

void	wait_childs(int n)
{
	int	i;
	int	status;

	i = 0;
	while (i < n)
	{
		wait(&status);
		if (WEXITSTATUS(status) && status != 0)
			g_error_status = WEXITSTATUS(status);
		else if (status == 0 && g_error_status != 1) // ????? check this!
				g_error_status = 0;
		i++;
	}
}

void	redirect(t_comm *tmp)
{
	int	err;

	err = 0;
	if (tmp->infile != FD_UNUSED)
		err = dup2(tmp->infile, STDIN_FILENO);
	if (err != 0)
		perror("1redirect:");
	err = 0;
	if (tmp->outfile != FD_UNUSED)
		err = dup2(tmp->outfile, STDOUT_FILENO);
	if (err != 0)
		perror("2redirect:");
}

int	*open_pipes(t_comm *tmp)
{
	int	i;
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

void	pipe_switch(int i, int kind, int *pipes, t_comm *tmp)
{
	if (tmp->count_node == 2)
	{
		if (kind == START && tmp->next != NULL)
			dup2(pipes[1], STDOUT_FILENO);
		else if (kind == END)
			dup2(pipes[0], STDIN_FILENO);
	}
	else
	{
		if (kind == START && tmp->next != NULL)
			dup2(pipes[2 * i + 1], STDOUT_FILENO);
		else if (kind == MIDDLE)
		{
			dup2(pipes[2 * i - 2], STDIN_FILENO);
			dup2(pipes[2 * i + 1], STDOUT_FILENO);
		}
		else if (kind == END)
			dup2(pipes[2 * i - 2], STDIN_FILENO);
	}
}

