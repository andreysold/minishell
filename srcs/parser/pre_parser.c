/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 17:29:35 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pipe_error(char *str, int len, int *i, int pipe_flag)
{
	if (pipe_flag == 0 || (*i) == 0)
		return (ft_return_error(PIPE));
	(*i)++;
	if (str[(*i)] == ' ')
	{
		while (str[(*i)] && str[(*i)] == ' ')
			(*i)++;
	}
	if (len == (*i) || str[(*i)] == '|')
		return (ft_return_error(PIPE));
	return (0);
}

int	ft_cond2(char *str, int *i)
{
	if (str[(*i)] == '\'')
	{
		ft_count_node2(str, i, '\'');
		if (str[(*i)] != '\'')
			return (ft_return_error(QUOT));
	}
	else if (str[(*i)] == '\"')
	{
		ft_count_node2(str, i, '\"');
		if (str[(*i)] != '\"')
			return (ft_return_error(QUOT));
	}
	return (0);
}

int	ft_cond1(char *str, int *i)
{
	int	count_red;
	int	count_back;

	count_red = 0;
	count_back = 0;
	if (str[(*i)] == '\'' || str[(*i)] == '\"')
	{
		if (ft_cond2(str, i) == -1)
			return (-1);
	}
	else if (str[(*i)] == '>')
	{
		if (ft_out_error(str, i, count_red) == -1)
			return (-1);
	}
	else if (str[(*i)] == '<')
	{
		if (ft_infile_error(str, i, count_back) == -1)
			return (-1);
	}
	return (0);
}
/** @param 
 * i[0] = i
 * i[1] = pipe_flag
 * i[2] = len
 **/

int	ft_lexer(char *str)
{
	int	i[3];

	ft_memset((void *)i, 0, sizeof(int *));
	i[2] = ft_strlen(str);
	while (i[0] < i[2])
	{
		if (str[i[0]] == '>' || str[i[0]] == '<'
			|| str[i[0]] == '\'' || str[i[0]] == '\"')
		{
			if (ft_cond1(str, &i[0]) == -1)
				return (-1);
		}
		else if (str[i[0]] == '|')
		{
			if (ft_pipe_error(str, i[2], &i[0], i[1]) == -1)
				return (-1);
			else
				continue ;
		}
		else if (str[i[0]] != ' ')
			i[1]++;
		i[0]++;
	}
	return (1);
}
