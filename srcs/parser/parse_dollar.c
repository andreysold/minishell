/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:46:09 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 13:59:05 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_func(char *str, int i, t_comm *lst)
{
	int		count;
	int		z;
	char	*tmp;
	char	*tmp2;

	count = 0;
	z = (i);
	count = ft_count_dol_value(str, &i);
	tmp = ft_substr(str, z, count);
	count = ft_len_value_dol(tmp, lst);
	return (count);
}

void	ft_is_key(char *str, int *i, int *t)
{
	while (str[*i] && (str[*i] != '\"' && str[*i] != '\''))
	{
		if (!(ft_iskey(str[(*i)])))
			break ;
		else
			t++;
		(*i)++;
	}
}

char	*ft_get_env_val(char *tmp, t_comm *lst, int *j)
{
	t_envp	*head;
	int		len;

	len = (int)ft_strlen(tmp);
	head = lst->e;
	while (head != NULL)
	{
		if (ft_strncmp(tmp, head->key, ft_strlen(head->key)) == 0
			&& ft_strlen(head->key) == len)
		{
			lst->fl = 1;
			free (tmp);
			tmp = ft_strdup(head->value);
			break ;
		}
		head = head->next;
	}
	if (lst->fl == 1)
		lst->tmp = ft_spasibo_norma(tmp, lst, j);
	free (tmp);
	return (lst->tmp);
}

char	*ft_shit_dollar(char *str, t_comm *lst, int *i, int *j)
{
	t_iter	*t;
	char	*tmp;

	t = malloc(sizeof(t_iter));
	if (!t)
		return (NULL);
	tmp = NULL;
	ft_memset((void *)t, 0, sizeof(t_iter));
	lst->fl = 0;
	t->z = (*i);
	while (str[(*i)] && str[*i] != '\"' && str[(*i)] != '\'')
	{
		if (!(ft_iskey(str[(*i)])))
			break ;
		else
			t->c++;
		(*i)++;
	}
	tmp = ft_substr(str, t->z, t->c);
	lst->tmp = ft_get_env_val(tmp, lst, j);
	free (t);
	return (lst->tmp);
}

int	ft_dol_str(char *str, t_comm *lst)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '?')
		{
			i++;
			count += ft_func(str, i, lst);
		}
		else if (str[i] == '$' && str[i + 1] == '?')
			count += 3;
		else
			count++;
		i++;
	}
	return (count);
}
