/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjonatho <wjonatho@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 15:04:47 by wjonatho          #+#    #+#             */
/*   Updated: 2021/11/01 17:11:03 by wjonatho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H

# include "../srcs/libft/libft.h"	//unistd : access dup2 close execve pipe; stdlib
# include "minishell.h"
# include <sys/fcntl.h>			//open
# include <sys/wait.h>			//waitpid
# include <stdio.h>				//perror
# include <sys/errno.h>

#define START	1
#define MIDDLE	2
#define END		3
void	error_n_exit(char *err_msg);
int		where_is_path(char **env);
char	*find_command_path(char *command_with_args, char **env);

#endif
