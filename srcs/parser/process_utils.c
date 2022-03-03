/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 17:53:17 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_skip_quotes(char *str, int *i, char c)
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != c)
	{
		if (str[(*i)] == ' ')
			str[(*i)] += 62;
		(*i)++;
	}
}

size_t	segments(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	len = 0;
	while (s[j])
	{
		while (s[j] != c && s[j])
		{
			j++;
			len++;
		}
		while (s[j] == c && s[j])
			j++;
		if (len > 0)
		{
			i++;
			len = 0;
		}
	}
	return (i);
}

char	*ft_global_value(t_comm *lst, int *i, int *j)
{
	char	*tmp;
	int		k;

	k = 0;
	(*i) += 2;
	tmp = ft_itoa(g_error_status);
	while (tmp[k])
		lst->tmp[(*j)++] = tmp[k++];
	free (tmp);
	return (lst->tmp);
}

char	*ft_new_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			ft_skip_quotes(str, &i, '\'');
		if (str[i] == '\"')
			ft_skip_quotes(str, &i, '\"');
		i++;
	}
	return (str);
}

t_comm	*ft_return_node(t_comm *lst)
{
	t_comm	*head;

	head = lst;
	while (lst)
	{
		if (lst->last_str)
		{
			lst->last_str = ft_destroy_space4(lst->last_str, lst);
			lst->cmd = ft_split(lst->last_str, ' ');
			free (lst->last_str);
			lst->cmd = ft_return_space(lst->cmd);
		}
		lst = lst->next;
	}
	lst = head;
	return (lst);
}
