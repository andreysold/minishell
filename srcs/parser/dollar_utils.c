/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 14:28:07 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_iskey(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*ft_spasibo_norma(char *tmp, t_comm *lst, int *j)
{
	int	l;

	l = 0;
	while (tmp[l])
		lst->tmp[(*j)++] = tmp[l++];
	return (lst->tmp);
}

int	ft_count_dol_value(char *str, int *i)
{
	int	count;

	count = 0;
	while (str[(*i)] && str[(*i)] != '\"')
	{
		if (!(ft_iskey(str[(*i)])))
			break ;
		else
			count++;
		(*i)++;
	}
	return (count);
}

int	ft_len_value_dol(char *tmp, t_comm *lst)
{
	t_envp	*head;
	size_t	len;

	len = ft_strlen(tmp);
	head = lst->e;
	lst->fl = 0;
	while (head)
	{
		if (ft_strncmp(head->key, tmp, ft_strlen(tmp)) == 0
			&& ft_strlen(head->key) == len)
		{
			free (tmp);
			tmp = ft_strdup(head->value);
			lst->fl = (int)ft_strlen(tmp);
			break ;
		}
		head = head->next;
	}
	lst->fl = (int)ft_strlen(tmp);
	free (tmp);
	return (lst->fl);
}
