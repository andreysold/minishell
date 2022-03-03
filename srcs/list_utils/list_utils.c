/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <galetha@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/20 14:12:03 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	env_list_free(t_envp **head)
{
	if ((*head)->key)
		free((*head)->key);
	if ((*head)->value)
		free((*head)->value);
	if ((*head))
		free((*head));
}

void	remove_element(t_envp **head, int location)
{
	t_envp	*tmp;
	t_envp	*prev;
	int		pos;

	pos = 0;
	tmp = *head;
	if (tmp != NULL && pos == location)
	{
		*head = tmp->next;
		env_list_free(&tmp);
		return ;
	}
	while (tmp != NULL && pos != location)
	{
		pos++;
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	prev->next = tmp->next;
	env_list_free(&tmp);
}

size_t	ft_listlen(t_envp *head)
{
	size_t	len;
	t_envp	*buf;

	len = 0;
	buf = head;
	while (buf)
	{
		buf = buf->next;
		len++;
	}
	return (len);
}
