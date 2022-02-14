/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 14:38:36 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/14 01:18:41 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isbuild(char *args)
{
	if (ft_strequ_hd(args, "exit"))
		return (1);
	else if (ft_strequ_hd(args, "env"))
		return (1);
	else if (ft_strequ_hd(args, "unset"))
		return (1);
	else if (ft_strequ_hd(args, "export"))
		return (1);
	else if (ft_strequ_hd(args, "pwd"))
		return (1);
	else if (ft_strequ_hd(args, "cd"))
		return (1);
	else if (ft_strequ_hd(args, "echo"))
		return (1);
	return (0);
}

int	exec_in_main(char **arg, t_env *env, int mod)
{
	int	ret;

	ret = 0;
	if (mod)
	{
		ret = ft_cmd(arg, &(env->envp));
		if (ret > 0)
			ret = exec_in_child(arg, env);
		// else if (ret == -1)
		// 	return (ret); // ft_printf("\033[1;33m4.M exec_in_main ret = %d\n\033[0m", ret); //! test
	}
	else
	{
		if (ft_cmd(arg, &(env->envp)) > 0)
		{
			if (!env->cmd_path)
			{
				perror("command not found: ");
				exit(1);
			}
			execve((env->cmd_path), arg, NULL);
		}
		exit(0);
	} // ft_printf("\033[1;33m4.M exec_in_main ret = %d\n\033[0m", ret); //! test
	return (ret);
}

int	exec_in_child(char **arg, t_env *env)
{
	int	pid;
	int	ret;

	ret = ft_isbuild(arg[0]);
	env->cmd_path = parse_cmd(env->path, arg);
	if (!ret && !env->cmd_path)
		return (CMD_ERROR);
	pid = fork();
	if (!pid)
	{
		if (ft_cmd(arg, &(env->envp)) > 0)
			execve((env->cmd_path), arg, NULL); // ft_printf("\033[1;33m4.E exec_in_child exit 0\n\033[0m"); //! test
		exit(0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		free(env->cmd_path);
	} // ft_printf("\033[1;33m4.E exec_in_child Return\n\033[0m"); //! test
	return (0);
}

char	*parse_cmd(char **path, char **cmd)
{
	char	*cmd_path;
	char	*tmp;
	size_t	i;

	tmp = ft_strjoin("/", cmd[0]);
	i = -1;
	while (path[++i])
	{
		cmd_path = ft_strjoin(path[i], tmp);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free(tmp);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free(tmp);
	return (NULL);
}
