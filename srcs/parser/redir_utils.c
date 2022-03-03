/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 14:38:30 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_del_space(char *str, int *i)
{
	int	k;

	k = 0;
	while (str[(*i)] && str[(*i)] != ' ')
	{
		k++;
		(*i)++;
	}
	return (k);
}

void	ft_skip_sp(char *str, int *i, int *begin)
{
	(*i)++;
	while (str[(*i)] == ' ')
		(*i)++;
	(*begin) = (*i);
	while (str[(*i)] && str[(*i)] != ' '
		&& str[(*i)] != '>' && str[(*i)] != '<')
		(*i)++;
}

char	*ft_new_sub(int i, t_comm *lst, char *str, int begin)
{
	t_envp	*head;
	char	*tmp;

	tmp = NULL;
	head = lst->e;
	lst->name = ft_key_file(str, &begin, i, lst->name);
	lst->name = ft_name_file(lst, tmp, head);
	return (lst->name);
}

char	*ft_key_file(char *str, int *begin, int len, char *name)
{
	int	k;

	k = 0;
	while ((*begin) < len)
	{
		if (str[(*begin)] == '^')
			str[(*begin)] -= 62;
		name[k] = str[(*begin)];
		k++;
		(*begin)++;
	}
	name[k] = '\0';
	return (name);
}

char	*ft_name_file(t_comm *lst, char *tmp, t_envp *head)
{
	int	i;

	i = 0;
	while (lst->name[i])
	{
		if (lst->name[i] == '$')
		{
			i++;
			tmp = ft_substr(lst->name, i, ft_strlen(lst->name) - i);
			while (head)
			{
				if (ft_strncmp(head->key, tmp, ft_strlen(head->key)) == 0)
				{
					free (tmp);
					free (lst->name);
					lst->name = ft_strdup(head->value);
					return (lst->name);
				}
				head = head->next;
			}
		}
		i++;
	}
	return (lst->name);
}
