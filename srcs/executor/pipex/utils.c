/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utlis.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:09:44 by wjonatho          #+#    #+#             */
/*   Updated: 2021/10/30 20:36:23 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/pipex.h"

void	error_n_exit(char *err_msg)
{
	perror(err_msg);
	exit(EXIT_FAILURE);
}

/// It could find string with PATH in env
/// @param env It will show UNIX enviroment, like command `env`
/// @return
/// position of line, where "PATH" had been found
/// or -1, if nothing found.
int	where_is_path(char **env)
{
	int	position;

	position = 0;
	while (env[position])
	{
		if (ft_strncmp("PATH", env[position], 4) == 0)
			return (position);
		position++;
	}
	return (-1);
}

static inline int	str_count(char **strings)
{
	int	i;

	i = 0;
	while (strings[i])
		i++;
	return (i);
}

char	*find_command_path(char *command, char **env)
{
	int		i;
	char	**splited;
	char	*path_to_command;
	char	**cmd;

	i = 0;
	splited = ft_split(env[where_is_path(env)] + 5, ':');
//	cmd = ft_split(command_with_args, ' ');
	i = 0;
	while (i < str_count(splited))
	{
		path_to_command = ft_strjoin(ft_strjoin(splited[i], "/"), command);
		if (access(path_to_command, R_OK) == 0)
		{
			/*printf("%sPID %d | '%s' path to the command %s\n", BLUE, getpid(), path_to_command, RESET);
			fflush(NULL);*/
			return (path_to_command);
		}
		i++;
	}

	return (ft_strdup(command));
}
