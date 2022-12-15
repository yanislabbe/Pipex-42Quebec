/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabbe <ylabbe@student.42quebec.c>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 19:00:27 by ylabbe            #+#    #+#             */
/*   Updated: 2022/11/29 23:54:07 by ylabbe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "projets.h"

void	quit_the_program(int error)
{
	if (error == 0)
		printf("Invalid number of arguments.\n");
	else if (error == 1)
		printf("Invalid infile.\n");
	else if (error == 2)
		printf("Invalid outfile.\n");
	else if (error == 3)
		printf("Invalid pipe.\n");
	else if (error == 4)
		printf("Invalid environment.\n");
	else if (error == 5)
		printf("Invalid here_doc.\n");
	else if (error == 6)
		printf("Command not found.\n");
	exit(1);
}

void	free_pipex(t_pipex pipex)
{
	pipex.i = 0;
	if (pipex.free_parent == 9 || pipex.free_pipe == 9)
	{
		close(pipex.f1);
		close(pipex.f2);
		if (pipex.here_doc)
			unlink(".heredoc_tmp");
		if (pipex.free_parent == 9)
		{
			while (pipex.cmd_paths[pipex.i])
				free(pipex.cmd_paths[pipex.i++]);
			free(pipex.cmd_paths);
		}
		free(pipex.pipe);
		if (pipex.free_pipe == 9)
			quit_the_program(3);
	}
	if (pipex.free_child == 9)
	{
		while (pipex.cmd_args[pipex.i])
			free(pipex.cmd_args[pipex.i++]);
		free(pipex.cmd_args);
		free(pipex.cmd);
	}
}

static char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, 0) == 0)
		{
			return (command);
		}
		free(command);
		paths++;
	}
	return (NULL);
}

static void	sub_dup2(int zero, int first)
{
	dup2(zero, 0);
	dup2(first, 1);
}

void	child(t_pipex pipex, char **argv, char **envp)
{
	pipex.pid = fork();
	if (!pipex.pid)
	{
		if (pipex.id == 0)
		{
			sub_dup2(pipex.f1, pipex.pipe[1]);
		}
		else if (pipex.id == pipex.cmd_nmbs - 1)
		{
			sub_dup2(pipex.pipe[2 * pipex.id - 2], pipex.f2);
		}
		else
		{
			sub_dup2(pipex.pipe[2 * pipex.id - 2], pipex.pipe[2 * pipex.id + 1]);
		}
		close_pipes(pipex);
		pipex.cmd_args = ft_split(argv[2 + pipex.here_doc + pipex.id], ' ');
		pipex.cmd = get_cmd(pipex.cmd_paths, pipex.cmd_args[0]);
		if (!pipex.cmd)
		{
			pipex.free_child = 9;
			free_pipex(pipex);
			quit_the_program(7);
		}
		execve(pipex.cmd, pipex.cmd_args, envp);
	}
}

void	here_doc(t_pipex *pipex, char *argv)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	if (file < 0)
		quit_the_program(5);
	while (1)
	{
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &buf) < 0)
		{
			exit(1);
		}
		if (!ft_strncmp(argv, buf, ft_strlen(argv) + 1))
		{
			break ;
		}
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->f1 = open(".heredoc_tmp", O_RDONLY);
	if (pipex->f1 < 0)
	{
		unlink(".heredoc_tmp");
		quit_the_program(5);
	}
}

static void	creat_pipes(t_pipex pipex)
{
	pipex.i = 0;
	while (pipex.i < pipex.cmd_nmbs - 1)
	{
		if (pipe(pipex.pipe + 2 * pipex.i) < 0)
		{
			pipex.free_parent = 9;
			free_pipex(pipex);
			pipex.free_parent = 0;
		}
		pipex.i++;
	}
}

void	close_pipes(t_pipex pipex)
{
	pipex.i = 0;
	while (pipex.i < pipex.pipe_nmbs)
	{
		close(pipex.pipe[pipex.i]);
		pipex.i++;
	}
}

void	outfile(t_pipex *pipex, char *argv)
{
	if (pipex->here_doc)
	{
		pipex->f2 = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0000664);
	}
	else
	{
		pipex->f2 = open(argv, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	}
	if (pipex->f2 < 0)
		quit_the_program(2);
}

void	infile(t_pipex *pipex, char **argv)
{
	if (!ft_strncmp("here_doc", argv[1], 9))
	{
		here_doc(pipex, argv[2]);
	}
	else
	{
		pipex->f1 = open(argv[1], O_RDONLY);
		if (pipex->f1 < 0)
			quit_the_program(1);
	}
}

char	*find_path(char **envp)
{
	while (ft_strncmp("PATH", *envp, 4))
	{
		envp++;
	}
	return (*envp + 5);
}

int	check_argv(t_pipex pipex, char *argv)
{
	if (argv && !ft_strncmp("here_doc", argv, 9))
	{
		pipex.here_doc = 1;
		return (6);
	}
	pipex.here_doc = 0;
	return (5);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	pipex.write_and_exit = 6;
	if (argc < check_argv(pipex, argv[1]))
		quit_the_program(0);
	pipex.free_pipe = 6;
	pipex.free_child = 6;
	pipex.free_parent = 6;
	pipex.i = 0;
	infile(&pipex, argv);
	outfile(&pipex, argv[argc - 1]);
	pipex.cmd_nmbs = argc - 3 - pipex.here_doc;
	pipex.pipe_nmbs = 2 * (pipex.cmd_nmbs - 1);
	pipex.pipe = malloc(sizeof(int) * pipex.pipe_nmbs + 1);
	if (!pipex.pipe)
		quit_the_program(3);
	pipex.env_path = find_path(envp);
	pipex.cmd_paths = ft_split(pipex.env_path, ':');
	if (!pipex.cmd_paths)
	{
		pipex.free_pipe = 9;
		free_pipex(pipex);
	}
	creat_pipes(pipex);
	pipex.id = -1;
	while (++(pipex.id) < pipex.cmd_nmbs)
	{
		child(pipex, argv, envp);
	}
	close_pipes(pipex);
	waitpid(-1, NULL, 0);
	pipex.free_parent = 9;
	free_pipex(pipex);
	return (0);
}
