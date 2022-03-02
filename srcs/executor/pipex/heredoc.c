/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/27 19:55:44 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	reading(t_comm **tmp)
{
	char	*str;
	int		here_len;

	here_len = (int)ft_strlen((*tmp)->here);
	while (1)
	{
		str = readline("> ");
		if (str == NULL)
			exit (EXIT_SUCCESS);
		else if (ft_strncmp(str, (*tmp)->here, here_len + 1) == 0)
			exit (EXIT_SUCCESS);
		ft_putendl_fd(str, (*tmp)->infile);
		free(str);
	}
}

void	heredoc(t_comm **tmp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid != 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (pid == 0)
	{
		signal(SIGINT, ff);
		signal(SIGQUIT, SIG_IGN);
		reading(tmp);
	}
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		g_error_status = 1;
	(*tmp)->infile = open(".tmp", O_RDONLY);
}
