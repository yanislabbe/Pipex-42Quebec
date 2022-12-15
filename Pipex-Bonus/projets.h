/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projets.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabbe <ylabbe@student.42quebec.c>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 19:00:19 by ylabbe            #+#    #+#             */
/*   Updated: 2022/11/29 23:53:44 by ylabbe           ###   ########.fr       */
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

typedef struct	s_pipex
{
	pid_t	pid;
	char	*env_path;
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	int		free_pipe;
	int		free_child;
	int		free_parent;
	int		write_and_exit;
	int		here_doc;
	int		cmd_nmbs;
	int		pipe_nmbs;
	int		*pipe;
	int		f1;
	int		f2;
	int		id;
	int		i;
}	t_pipex;



void	quit_the_program(int error);
void	infile(t_pipex *pipex, char **argv);
void	outfile(t_pipex *pipex, char *argv);
void	here_doc(t_pipex *pipex, char *argv);
int		check_argv(t_pipex pipex, char *argv);

//void	quit_the_program(t_pipex pipex);
void	free_pipex(t_pipex pipex);
void	child(t_pipex pipex, char **argv, char **envp);
void	close_pipes(t_pipex pipex);
char	*find_path(char **envp);
//int		args_in(t_pipex pipex, char **arg);

#endif
