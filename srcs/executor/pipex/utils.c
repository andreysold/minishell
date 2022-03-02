/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:09:44 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 13:19:46 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/pipex.h"

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

	if (command == NULL)
		return (ft_strdup(""));
	i = where_is_path(env);
	if (i == -1)
		return (ft_strdup(command));
	splited = ft_split(env[where_is_path(env)] + 5, ':');
	i = 0;
	while (i < str_count(splited))
	{
		path_to_command = ft_strjoin(ft_strjoin(splited[i], "/"), command);
		if (access(path_to_command, R_OK) == 0)
			return (path_to_command);
		i++;
	}
	return (ft_strdup(command));
}
