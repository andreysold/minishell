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
		write(2, str, ft_strlen(str));
	g_error_status = 258;
	return (-1);
}


int	ft_lexer(char *str)
{
	int i;
	int len;
	int count_red;
	int	count_back;
	int pipe_flag;
	
	pipe_flag = 0;
	count_red = 0;
	count_back = 0;
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
		else if (str[i] == '\"')
		{
			ft_count_node2(str, &i, '\"');
			if (str[i] != '\"')
				return (ft_return_error("bash: syntax error in unclosed quoters\n"));
		}
		else if (str[i] == '|')
		{
			if (pipe_flag == 0 || i == 0)
				return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
			i++;
			if (str[i] == ' ')
			{
				while (str[i] && str[i] == ' ')
					i++;
			}
			if (len == i || str[i] == '|')
				return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
			else 
				continue ;
		}
		else if (str[i] == '>')
		{
			i++;
			count_red = 1;
			count_red += ft_count_redir(str, &i, '>');
			while (str[i] && str[i] == ' ')
				i++;
			if (str[i] == '<')
				return (ft_return_error("bash: syntax error near unexpected token `<'\n"));
			if (count_red == 3)
				return (ft_return_error("bash: syntax error near unexpected token `>'\n"));
			else if (count_red > 3)
				return (ft_return_error("bash: syntax error near unexpected token `>>'\n"));
			if (str[i] == '|' || i == len)
				return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
			
		}
		else if (str[i] == '<')
		{
			i++;
			count_back = 1;
			count_back += ft_count_redir(str, &i, '<');
			if (str[i] == '>')
				return (ft_return_error("bash: syntax error near unexpected token `>'\n"));
			if (count_back == 4)
				return (ft_return_error("bash: syntax error near unexpected token `<'\n"));
			else if (count_back == 5)
				return (ft_return_error("bash: syntax error near unexpected token `<<'\n"));
			else if (count_back > 5)
				return (ft_return_error("bash: syntax error near unexpected token `<<<'\n"));
		}
		else if (str[i] != ' ')
			pipe_flag++;
		i++;
	}
	return (1);
}