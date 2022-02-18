#include "../../includes/minishell.h"


void	ft_skip_space(char *str, int *i)
{
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
}

int	ft_count_redir(char *str, int *i, char c)
{
	int count_red;

	count_red = 0;
	while (str[(*i)] && str[(*i)] == c)
	{
		(*i)++;
		count_red++;
	}
	return (count_red);
}

int	ft_return_error(char *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	g_error_status = 258;
	// g_error = ?
	return (-1);
}


// int ft_quotes_error(char *str, int *i)
// {
// 	if (str[(*i)] == ' ')
// 		ft_skip_space(str, i);
// 	if (str[(*i)] == '\'')
// 	{
// 		ft_count_node2(str, i, '\'');
// 		if (str[(*i)] != '\'')
// 			return (ft_return_error("bash: syntax error in unclosed quoters\n"));
// 	}
// 	if (str[(*i)] == '\"')
// 	{
// 		ft_count_node2(str, i, '\"');
// 		if (str[(*i)] != '\"')
// 			return (ft_return_error("bash: syntax error in unclosed quoters\n"));
// 	}
// 	return (0);
// }

// int ft_pipe_error(char *str, int *i, int len)
// {
// 	if (str[(*i)] == '|')
// 	{
// 		(*i)++;
// 		ft_skip_space(str, i);
// 		if (str[(*i)] == '|')
// 			return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
// 		else if ((*i) == len)
// 			return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
// 	}

// 	return (0);
// }


int	ft_lexer(char *str)
{
	int i;
	int len;
	int count_red;

	count_red = 0;
	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if (str[i] == '\'')
		{
			ft_count_node2(str, &i, '\'');
			if (str[i] != '\'')
				return (ft_return_error("bash: syntax error in unclosed quoters\n"));
		}
		if (str[i] == '\"')
		{
			ft_count_node2(str, &i, '\"');
			if (str[i] != '\"')
				return (ft_return_error("bash: syntax error in unclosed quoters\n"));
		}
		if (str[i] == '|')
		{
			i++;
			ft_skip_space(str, &i);
			if (str[i] == '|')
				return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
			else if (i == len)
				return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
		}
		if (str[i] == '>')
		{
			i++;
			if (str[i] == '<')
				return (ft_return_error("bash: syntax error near unexpected token `<'\n"));
			count_red = 1;
			if (str[i] == '>')
			{
				count_red += ft_count_redir(str, &i, '>');
				if (count_red > 3)
					return (ft_return_error("bash: syntax error near unexpected token `<'\n"));
			}
			if (str[i] == '|')
				return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
			else if (i == len)
				return (ft_return_error("bash: syntax error near unexpected token `newline'\n"));
		}
		i++;
	}
	return (1);
}