#include "../../includes/minishell.h"

void ft_count_node2(char *str, int *i, char c)
{
    (*i)++;
    while (str[(*i)] && str[(*i)] != c)
        (*i)++;
}

int ft_count_node(char *str) // func for count nodes
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == '\'')
            ft_count_node2(str, &i, '\'');
        else if (str[i] == '\"')
            ft_count_node2(str, &i, '\"');
        else if (str[i] == '|')
            count++;
        i++;
    }
    return (count + 1);
}

char *ft_tream(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '^')
			str[i] -= 62;
		i++;
	}
	return (str);
}

char **ft_return_space(char **str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i])
	{
		str[i] = ft_tream(str[i]);
		i++;
	}
	return (str);
}

