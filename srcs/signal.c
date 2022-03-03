/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/27 19:55:44 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ff(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	g_error_status = 1;
	exit(g_error_status);
}

void	handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \b\b\n", 5);
	rl_on_new_line();
//	rl_replace_line("", 1);
	rl_redisplay();
	g_error_status = 1;
}
