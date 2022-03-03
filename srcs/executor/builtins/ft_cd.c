/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/20 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	update_value(t_comm *lst, char *key)
{
	char	*clean;
	int		location;
	char	*buf;

	buf = NULL;
	location = locate_env_key(lst->e, key);
	if (location != -1)
	{
		clean = getcwd(buf, 0);
		upd_env_value(lst->e, clean, location);
		if (clean)
			free(clean);
	}
}

static inline void	find_home_path(t_comm *lst, char **new_path, int pos)
{
	char	*home;

	home = get_env_value(lst->e, pos);
	pos = locate_env_key(lst->e, "PWD");
	if (pos != -1)
		upd_env_value(lst->e, home, pos);
	*new_path = ft_strdup(home);
}

static inline int	search_path(t_comm *lst, char	**new_path)
{
	int		pos;

	if (lst->cmd[1] == NULL)
	{
		pos = locate_env_key(lst->e, "HOME");
		if (pos == -1)
		{
			ft_putendl_fd("bash: cd: HOME not set", 2);
			return (EXIT_FAILURE);
		}
		else
			find_home_path(lst, new_path, pos);
	}
	else if (!ft_strncmp(lst->cmd[1], "\0", 1))
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
		g_error_status = 1;
	}
	if (new_path)
		free(new_path);
	update_value(lst, "PWD");
	g_error_status = 0;
	return (EXIT_SUCCESS);
}
