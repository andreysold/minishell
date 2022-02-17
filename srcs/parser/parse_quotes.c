#include "../../includes/minishell.h"

char *ft_one_quotes(char *str, t_comm *lst, int *i, int *j)
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != '\'')
		lst->tmp[(*j)++] = str[(*i)++];
	if (str[(*i)] == '\'')
		(*i)++;
	return (lst->tmp);
}

char *ft_two_quotes(char *str,  t_comm *lst, int *i, int *j) // no norm
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != '\"' && str[(*i)] != '\'')
	{
		if (str[(*i)] == '$' && str[(*i) + 1] != ' ')
		{
			(*i)++;
			lst->tmp = ft_shit_dollar(str, lst, i, j);
			continue ;
		}
		if (str[(*i)] != '\"')
			lst->tmp[(*j)++] = str[(*i)];
		(*i)++;
	}
	if (str[(*i)] != '\"')
	{
		lst->tmp = ft_strjoin(lst->tmp, "");
	}
	return (lst->tmp);
}
