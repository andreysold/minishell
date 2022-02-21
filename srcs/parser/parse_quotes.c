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
	int flag;

	flag = (*i);
	(*i)++;
	while (str[(*i)] && str[(*i)] != '\"')
	{
		if (str[(*i)] == '$' && str[(*i) + 1] == '?')
		{
			// (*i)++;
			lst->tmp = ft_global_value(str, lst, i, j);
			continue ;
		}
		else if (str[(*i)] == '$' && str[(*i) + 1] != ' ')
		{
			(*i)++;
			lst->tmp = ft_shit_dollar(str, lst, i, j);
			continue ;
		}
		else
			lst->tmp[(*j)++] = str[(*i)++];
	}
	if (str[(*i)] == '\"')
		(*i)++;
	return (lst->tmp);
}
