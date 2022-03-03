/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:08:07 by wjonatho          #+#    #+#             */
/*   Updated: 2022/03/03 18:07:16 by galetha          ###   ########.fr       */
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
	location = locate_env_key(lst->e, "PWD");
	if (location != -1)
		upd_env_value(lst->e, pwd, location);
	if (pwd)
		free(pwd);
	g_error_status = 0;
	return (EXIT_SUCCESS);
}
