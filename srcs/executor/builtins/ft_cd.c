/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/02 12:55:27 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	update_value(t_comm *lst, char *key)
{
	char	*clean;
	int		location;
	char	*buf;

	buf = NULL;
	location = locate_env_key(lst->e, key, 0);
	if (location != -1)
	{
		clean = getcwd(buf, 0);
		upd_env_value(lst->e, clean, location, 0);
		if (clean)
			free(clean);
	}
}

static inline void	find_home_path(t_comm *lst, char **new_path, int pos)
{
	char	*home;

	home = get_env_value(lst->e, pos, 0);
	pos = locate_env_key(lst->e, "PWD", 0);
	if (pos != -1)
		upd_env_value(lst->e, home, pos, 0);
	*new_path = ft_strdup(home);
}

static inline int	search_path(t_comm *lst, char	**new_path)
{
	int		pos;
	char	*home;

	if (lst->cmd[1] == NULL)
	{
		pos = locate_env_key(lst->e, "HOME", 0);
		if (pos == -1)
		{
			ft_putendl_fd("bash: cd: HOME not set", 2);
			return (EXIT_FAILURE);
		}
		else
			find_home_path(lst, new_path, pos);
	}
	else if (!ft_strncmp(lst->cmd[1], "\0", 1)) //FIXME doesn't work
		return (EXIT_FAILURE);
	else
		*new_path = ft_strdup(lst->cmd[1]);
	return (EXIT_SUCCESS);
}

/**	copy PATH
 *	add new key-value as OLD_PATH
 *	change PATH to new_path
 *	chdir(new_path) //
**/
int	ft_cd(t_comm *lst)
{
	char	*new_path;

	update_value(lst, "OLDPWD");
	if (search_path(lst, &new_path))
		return (EXIT_FAILURE);
	if (chdir(new_path) == -1)
	{
		if (lst->cmd[1])
			printf("bash: cd: %s: %s\n", lst->cmd[1], strerror(errno));
		else
			printf("bash: cd: %s\n", strerror(errno));
	}
	else
	{
		if (new_path)
			free(new_path);
	}
	update_value(lst, "PWD");
	return (EXIT_SUCCESS);
}
