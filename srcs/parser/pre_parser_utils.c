/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 16:58:41 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_skip_space(char *str, int *i)
{
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
}

int	ft_count_redir(char *str, int *i, char c)
{
	int	count_red;

	count_red = 0;
	while (str[(*i)] && str[(*i)] == c)
	{
		(*i)++;
		count_red++;
	}
	return (count_red);
}

int	ft_return_error(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	g_error_status = 258;
	return (-1);
}

int	ft_infile_error(char *str, int *i, int count_back)
{
	(*i)++;
	count_back = 1;
	count_back += ft_count_redir(str, i, '<');
	if (str[(*i)] == '>')
		return (ft_return_error(RED1));
	if (count_back == 3)
		return (ft_return_error(RED3));
	if (count_back == 4)
		return (ft_return_error(RED3));
	else if (count_back == 5)
		return (ft_return_error(RED4));
	else if (count_back > 5)
		return (ft_return_error(RED5));
	return (0);
}

int	ft_out_error(char *str, int *i, int count_red)
{
	int	len;

	len = ft_strlen(str);
	(*i)++;
	count_red = 1;
	count_red += ft_count_redir(str, i, '>');
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
	if (str[(*i)] == '<')
		return (ft_return_error(RED3));
	if (count_red == 3)
		return (ft_return_error(RED1));
	else if (count_red > 3)
		return (ft_return_error(RED2));
	if (str[(*i)] == '|' || (*i) == len)
		return (ft_return_error(PIPE));
	return (0);
}
