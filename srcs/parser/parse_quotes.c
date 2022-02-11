#include "../../includes/minishell.h"

char *ft_one_quotes(char *str, char *tmp, int *i, int *j)
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != '\'')
		tmp[(*j)++] = str[(*i)++];
	if (str[(*i)] == '\'')
		(*i)++;
	return (tmp);
}

char *ft_two_quotes(char *str, char **env, char *tmp, int *i, int *j) // no norm
{
	(*i)++;
	while (str[(*i)] && str[(*i)] != '\"')
	{
		if (str[(*i)] == '$' && str[(*i) + 1] != ' ')
		{
			(*i)++;
			tmp = ft_shit_dollar(str, env, tmp, i, j);
			continue ;
		}
		if (str[(*i)] != '\"')
			tmp[(*j)++] = str[(*i)];
		(*i)++;
	}
	return (tmp);
}
