/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabbe <ylabbe@student.42quebec.c>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 19:00:27 by ylabbe            #+#    #+#             */
/*   Updated: 2022/11/15 19:08:26 by ylabbe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "projets.h"

void	quit_or_and_free(t_pipex pipex)
{
	if (pipex.free == 9)
	{
		pipex.i = 0;
		while (pipex.mycmdargs[pipex.i])
			free(pipex.mycmdargs[pipex.i++]);
		free(pipex.mycmdargs);
		free(pipex.cmd);
	}
	write(2, "Error\n", 7);
	exit(1);
}

void	child_one(t_pipex pipex, char *argv[], char *envp[])
{
	dup2(pipex.end[1], 1);
	close(pipex.end[0]);
	dup2(pipex.f1, 0);
	pipex.mycmdargs = ft_split(argv[2], ' ');
	while (*pipex.mypaths)
	{
		pipex.tmp = ft_strjoin(*pipex.mypaths, "/");
		pipex.command = ft_strjoin(pipex.tmp, pipex.mycmdargs[0]);
		free(pipex.tmp);
		if (access(pipex.command, 0) == 0)
		{
			pipex.cmd = pipex.command;
			break ;
		}
		free(pipex.command);
		pipex.mypaths++;
	}
	if (!pipex.cmd)
	{
		pipex.free = 9;
		quit_or_and_free(pipex);
	}
	execve(pipex.cmd, pipex.mycmdargs, envp);
}

void	child_two(t_pipex pipex, char *argv[], char *envp[])
{
	dup2(pipex.end[0], 0);
	close(pipex.end[1]);
	dup2(pipex.f2, 1);
	pipex.mycmdargs = ft_split(argv[3], ' ');
	while (*pipex.mypaths)
	{
		pipex.tmp = ft_strjoin(*pipex.mypaths, "/");
		pipex.command = ft_strjoin(pipex.tmp, pipex.mycmdargs[0]);
		free(pipex.tmp);
		if (access(pipex.command, 0) == 0)
		{
			pipex.cmd = pipex.command;
			break ;
		}
		free(pipex.command);
		pipex.mypaths++;
	}
	if (!pipex.cmd)
	{
		pipex.free = 9;
		quit_or_and_free(pipex);
	}
	execve(pipex.cmd, pipex.mycmdargs, envp);
}

void	run_pipex(t_pipex pipex, char *argv[], char *envp[])
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	pipex.path_from_envp = (*envp + 5);
	pipex.mypaths = ft_split(pipex.path_from_envp, ':');
	pipex.child1 = fork();
	if (pipex.child1 < 0)
		quit_or_and_free(pipex);
	if (pipex.child1 == 0)
		child_one(pipex, argv, envp);
	pipex.child2 = fork();
	if (pipex.child2 < 0)
		quit_or_and_free(pipex);
	if (pipex.child2 == 0)
		child_two(pipex, argv, envp);
	close(pipex.end[0]);
	close(pipex.end[1]);
	waitpid(pipex.child1, NULL, 0);
	waitpid(pipex.child2, NULL, 0);
	pipex.i = 0;
	close(pipex.f1);
	close(pipex.f2);
	while (pipex.mypaths[pipex.i])
		free(pipex.mypaths[pipex.i++]);
	free(pipex.mypaths);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	pipex.free = 0;
	if (argc != 5)
		quit_or_and_free(pipex);
	pipex.cmd = NULL;
	pipex.f1 = open(argv[1], O_RDONLY);
	pipex.f2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex.f1 < 0 || pipex.f2 < 0)
		quit_or_and_free(pipex);
	if (pipe(pipex.end) < 0)
		quit_or_and_free(pipex);
	run_pipex(pipex, argv, envp);
	return (0);
}
