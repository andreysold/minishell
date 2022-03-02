/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                            :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/20 17:53:20 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_comm *lst)
{
	char	*pwd;
	char	*buf;
	int		location;

	buf = NULL;
	pwd = getcwd(buf, 0);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	location = locate_env_key(lst->e, "PWD", 0);
	if (location != -1)
		upd_env_value(lst->e, pwd, location, 0);
	if (pwd)
		free(pwd);
	return (EXIT_SUCCESS);
}
