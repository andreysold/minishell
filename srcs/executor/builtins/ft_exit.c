#include	"../../../includes/minishell.h"

int ft_count_strings(t_comm *lst)
{
	int count;

	count = 0;
	while (lst->cmd[count])
		count++;
	return (count);
}

int	ft_new_value_error(char *str)
{
	int i;
	long long 	n;
	int new_val;
	int sign;
	unsigned long long		max;

	max = 9223372036854775807;
	sign = 1;
	i = 0;
	n = 0;
	if (str && str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str && ft_isdigit(str[i]))
	{
		n = 10 * n + (str[i] - 48);
		i++;
	}
	n *= sign;
	// printf("%lld\n", n);
	if (n > max && sign != -1)
	{
		printf("A\n");
		write(2, "bash: exit: ",12);
		write(2, str, ft_strlen(str));
		write(2, " numeric argument required\n", 28);
		g_error_status = 255;
		return (g_error_status);
	}
	// else if (n < min && sign)
	// {
	// 	printf("A\n");
	// 	write(2, "bash: exit: ",12);
	// 	write(2, str, ft_strlen(str));
	// 	write(2, " numeric argument required\n", 28);
	// 	return (255);
	// }
	n = n % 256;
	while (n < 0)
		n = n + 256;
	return (n);
}

int	ft_check_exit_numeric(t_comm *lst)
{
	t_comm	*head;
	int		count_min;
	int		j;
	j = 0;
	count_min = 0;
	while (lst->cmd[1][j])
	{
		if (lst->cmd[1][j] == '-' && count_min == 0)
		{
			count_min = 1;
			j++;
		}
		if (!(ft_isdigit(lst->cmd[1][j])))
		{
			printf("A\n");
			write(2, "bash: exit: ", 12);
			write(2, lst->cmd[1], ft_strlen(lst->cmd[1]));
			write(2, ": numeric argument required\n", 29);
			g_error_status = 255;
			exit (g_error_status);
		}
		j++;
	}
	g_error_status = ft_new_value_error(lst->cmd[1]);
	return (g_error_status);
}

int	ft_exit_many_args(t_comm *lst)
{
	int i;

	i = 0;
	while (lst->cmd[1][i])
	{
		if (ft_isalpha(lst->cmd[1][i]))
		{
			printf("A\n");
			write(2, "bash: exit: ", 12);
			write(2, lst->cmd[1], ft_strlen(lst->cmd[1]));
			write(2, ": numeric argument required\n", 29);
			g_error_status = 255;
			return (g_error_status);
		}
		i++;
	}

	write(2, "bash: exit: ", 12);
	write(2, ": too many arguments\n", 22);
	g_error_status = 1;
	return (g_error_status);
}

int	ft_exit(t_comm *lst)
{
	int shlvl;
	char *tmp;
	int count;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	count = ft_count_strings(lst);
	if (count == 2)
		g_error_status = ft_check_exit_numeric(lst);
	else if (count > 2)
		ft_exit_many_args(lst);
	else
		exit (0);
	// exit (0);
	return (EXIT_FAILURE);
}