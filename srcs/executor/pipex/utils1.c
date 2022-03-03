/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.42.fr>           +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:09:44 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/21 18:20:21 by wjonatho          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bash_error(char *first_part, char *cmd, char *last_part)
{
	ft_putstr_fd(first_part, 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(last_part, 2);
	g_error_status = 127;
}

void	close_in_out_file(t_comm *tmp)
{
	int	err;

	err = 0;
	if (tmp->outfile != -2)
		err = close(tmp->outfile);
	if (tmp->infile != -2)
		err = close(tmp->infile);
}

void	close_pipes(int *pipes, int count_node)
{
	int	i;
	int	n;

	i = 0;
	n = 2 * (count_node - 1);
	while (i < n)
		close(pipes[i++]);
}
