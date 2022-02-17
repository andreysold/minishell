
int ft_redir_error(char *str, int *i, int *count_red, int len)
{
	(*i)++;
	if (str[(*i)] == '>')
			return (ft_return_error("bash: syntax error near unexpected token `>'\n"));
	if (str[(*i)] == '<')
	{
		(*count_red) = 1;
		(*count_red) += ft_count_redir(str, i, '<');
		if ((*count_red) == 4)
			return (ft_return_error("bash: syntax error near unexpected token `<'\n"));
		else if ((*count_red) == 5)
			return (ft_return_error("bash: syntax error near unexpected token `<<'\n"));
		else if ((*count_red) > 5)
			return (ft_return_error("bash: syntax error near unexpected token `<<<'\n"));
		ft_skip_space(str, i);
		if (str[(*i)] == '|')
			return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
	}
	if (str[(*i)] == ' ')
		ft_skip_space(str, i);
	if (str[(*i)] == '|')
		return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
	else if ((*i) == len)
		return (ft_return_error("bash: syntax error near unexpected token `newline'\n"));
	return (0);
}

int ft_redir_error2(char *str, int *i, int *count_red, int len)
{
	(*i)++;
	if (str[(*i)] == '<')
			return (ft_return_error("bash: syntax error near unexpected token `<'\n"));
	(*count_red) = 1;
	if (str[(*i)] == '>')
	{
		(*count_red) += ft_count_redir(str, i, '>');
		if ((*count_red)> 3)
			return (ft_return_error("bash: syntax error near unexpected token `>>'\n"));
		else if ((*count_red)== 3)
			return (ft_return_error("bash: syntax error near unexpected token `>'\n"));
	}
	if (str[(*i)] == ' ')
		ft_skip_space(str, i);
	if (str[(*i)] == '|')
		return (ft_return_error("bash: syntax error near unexpected token `|'\n"));
	else if ((*i) == len)
		return (ft_return_error("bash: syntax error near unexpected token `newline'\n"));
	return (0);
}
