/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projets.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabbe <ylabbe@student.42quebec.c>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 19:00:19 by ylabbe            #+#    #+#             */
/*   Updated: 2022/11/15 19:10:02 by ylabbe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROJETS_H
# define PROJETS_H

# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include "./ft_printf/ft_printf.h"
# include <unistd.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>

typedef struct s_pipex
{
	pid_t	child1;
	pid_t	child2;
	char	*path_from_envp;
	char	**mypaths;
	char	**mycmdargs;
	char	*cmd;
	char	*command;
	char	*tmp;
	int		end[2];
	int		f1;
	int		f2;
	int		i;
	int		free;
}	t_pipex;

void	child_one(t_pipex pipex, char *argv[], char *envp[]);
void	child_two(t_pipex pipex, char *argv[], char *envp[]);
void	quit_or_and_free(t_pipex pipex);
void	run_pipex(t_pipex pipex, char *argv[], char *envp[]);

#endif
