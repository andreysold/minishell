/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 17:56:40 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_one_redir(t_comm *lst, char *str, int *i, int *begin)
{
	ft_skip_sp(str, i, begin);
	lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
	if (!lst->name)
		return (-1);
	lst->name = ft_new_sub(*i, lst, str, *begin);
	lst->outfile = open(lst->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!lst->outfile)
		return (-1);
	if (lst->outfile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
		g_error_status = -1;
		free (lst->name);
		return (-1);
	}
	free (lst->name);
	return (0);
}

int	ft_back_redir(t_comm *lst, char *str, int *i, int *begin)
{
	ft_skip_sp(str, i, begin);
	lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
	if (!lst->name)
		return (-1);
	lst->name = ft_new_sub(*i, lst, str, *begin);
	lst->infile = open(lst->name, O_RDONLY, 0644);
	if (!lst->infile)
		return (-1);
	if (lst->infile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
		g_error_status = 1;
		free (lst->name);
		return (-1);
	}
	free (lst->name);
	return (0);
}

int	ft_add_redir(t_comm *lst, char *str, int *i, int *begin)
{
	(*i)++;
	ft_skip_sp(str, i, begin);
	lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin) + 1));
	if (!lst->name)
		return (-1);
	lst->name = ft_new_sub(*i, lst, str, *begin);
	lst->outfile = open(lst->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (!lst->outfile)
		return (-1);
	if (lst->outfile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
		g_error_status = -1;
		free (lst->name);
		return (-1);
	}
	free (lst->name);
	return (0);
}

int	ft_herdok(t_comm *lst, char *str, int *i)
{
	int	beg;
	int	k;

	(*i) += 2;
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
	beg = (*i);
	k = ft_del_space(str, i);
	lst->here = ft_substr(str, beg, k);
	lst->infile = open(".tmp", O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (lst->infile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
		g_error_status = -1;
		return (-1);
	}
	return (0);
}

char	*ft_open_file(char *str, int *i, int *j, t_comm *lst)
{
	int	begin;

	begin = 0;
	while (str[(*i)])
	{
		if (str[(*i)] == '>' && str[(*i) + 1] != '>')
			lst->flag_error = ft_one_redir(lst, str, i, &begin);
		else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
			lst->flag_error = ft_add_redir(lst, str, i, &begin);
		else if (str[(*i)] == '<' && str[(*i) + 1] != '<')
			lst->flag_error = ft_back_redir(lst, str, i, &begin);
		else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
			lst->flag_error = ft_herdok(lst, str, i);
		else
			lst->tmp[(*j)++] = str[(*i)++];
	}
	return (lst->tmp);
}
