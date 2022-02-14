#include "../../includes/minishell.h"



int	ft_lexer(char *str)
{
	int i;
	int len;
    int j;
	int er;

	er = 0;
    j = 0;
	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if (str[i] == '\'')
		{
			ft_count_node2(str, &i, '\'');
			if (str[i] != '\'')
			{
				write(1, "bash: syntax error in unclosed quoters\n", 40);
				return (-1);
			}
		}
		if (str[i] == '\"')
		{
			ft_count_node2(str, &i, '\"');
			if (str[i] != '\"')
			{
				write(1, "bash: syntax error in unclosed quoters\n", 40);
				return (-1);
			}
		}
		if (str[i] == '|')
		{
			i++;
			while (str[i] && str[i] == ' ')
				i++;
			if (str[i] == '|')
			{
				write(1, "bash: syntax error in empty node\n", 34);
				return (-1);
			}
			if (i == len)
			{
				write(1, "bash: syntax error in empty node\n", 34);
				return (-1);
			}
			else
				continue ;
		}
		i++;
	}
	return (1);
}