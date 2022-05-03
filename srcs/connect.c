/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 17:02:01 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_path(t_env *env, char **args, int mod)
{
	if (mod != DESTROY_SET && args)
	{
		if (ft_strchr(args[0], '/'))
		{	
			if (access(args[0], F_OK | X_OK) != 0)
				env->cmd_path = 0;
			else
				env->cmd_path = ft_strdup(args[0]);
		}
		else
			env->cmd_path = parse_cmd(env->path, args);
		return ;
	}
	if (env->cmd_path && mod == DESTROY_SET)
	{
		free(env->cmd_path);
		env->cmd_path = NULL;
	}
}

int	redir_manag(t_redir *to_redir, t_env *env)
{
	int	ret;

	ret = 0;
	if (to_redir->type == REDIR_L)
		ret = redir_to_stdin(to_redir->word);
	else if (to_redir->type == REDIR_LL)
	{
		dup2(env->oldstdin, 0);
		ret = redir_heredoc(to_redir->word);
	}
	else if (to_redir->type == REDIR_RR)
		ret = redir_to_stdout(to_redir->word, O_APPEND);
	else if (to_redir->type == REDIR_R)
		ret = redir_to_stdout(to_redir->word, O_TRUNC);
	if (ret == CTRL_C)
		return (ret);
	else if (ret)
		return (all_error(ret, to_redir->word));
	return (0);
}

int	redir_lst(t_list *redir_lst, t_env *env)
{
	t_redir	*to_redir;
	int		ret;

	ret = 0;
	while (redir_lst)
	{
		to_redir = (t_redir *)redir_lst->content;
		ret = redir_manag(to_redir, env);
		if (ret)
			return (ret);
		redir_lst = redir_lst->next;
	}
	return (SUCCESS);
}

int	exec_cmds(t_list *cmds, t_env *env)
{
	int	ret;

	ret = 0;
	tty_show_ctrl();
	if (cmds->next)
		ret = exec_pipe(cmds, env);
	else
		ret = exec_simple_cmd((t_cmd *)(cmds->content), env);
	return (ret);
}
