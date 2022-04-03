/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 19:51:20 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 04:31:09 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include "../libft/libft.h"
# include "cmds.h"
# include <sys/types.h>
# include <sys/wait.h>

# define PROMPT		"minishell$> "
# define QUOTE		39
# define DQUOTE		34
# define BACKSLASH	92

typedef struct s_global
{
	long long	g_exit_status; // ! A garder ?
	long long	g_check_hd; // ! A garder ?
}	t_global;

t_global	g_set;

enum	e_err
{
	SUCCESS,
	BUILD_ERROR,
	TMP,
	BF_ERROR,
	OP_ERROR,
	DUP_ERROR,
	OUT_ERROR,
	CTRL_C,
	CMD_ERROR = 127
};

enum	e_mod
{
	IN_ENV,
	OUT_OF_ENV,
	IN_PIPE,
	LAST_PIPE_BLOCK,
	IN_MAIN,
	IN_CHILD,
	SET,
	DESTROY_SET
};

enum	e_redir_type
{
	WORD,
	PIPE,
	REDIR_L,
	REDIR_LL,
	REDIR_R,
	REDIR_RR
};
typedef struct s_token
{
	int		type;
	char	*val;
}	t_token;
typedef struct s_env
{
	t_list	*envp;
	t_list	*ex_env;
	char	**nbtfke;
	char	**path;
	char	*cmd_path;
	int		oldstdin;
	int		oldstdout;
	int		out_check;
	int		child;
}		t_env;

typedef struct	s_g
{
	int	in_hd;
	int	stop_hd;
}	t_g;

//		builtins.c
void	cd_to_envvar(t_list **env, char *var);
void	my_chdir(char *path, t_list **env);
void	ft_cd(char **args, t_list **env);
int		exec_build_in_env(char **args, t_env *env);
void	ft_echo(char **arg);

//		check.c
int		check_the_build_for_env(char *args);
int		check_unset_path(char **path, t_env *env);
int		ft_isbuild(char *args);

//		connect_utils.c                         
void	set_path(t_env *env, char **args, int mod);
void	reset_routine(t_env *env, int ret);
int		redir_manag(t_redir *to_redir, t_env *env);
int		redir_lst(t_list *redir_lst, t_env *env);
int		launch_exec(t_env *env, t_cmd *cmd, int mod);

//		connect.c
int		connecting_fct(t_list *cmd, t_env *env);

//		dev.c
void	print_msg_err(char	*msg, char *arg);
void	print_strs(char **strs);
void	print_cmd(void *ptr);
void	print_redir(void *ptr);
void	print_tokens(t_list *tokens);

//		env_utils.c
int		is_valide_var(char *str, int mod);
int		ft_strc_index(char *str, int c);
void	delref(t_list **lst, void *data_ref);
void	add_ref(t_list **lst, void *data_ref, int idx);

//		env.c
void	ft_env(char **args, t_env *env);
int		manag_exec_in_env(t_list *cmd);
int		ft_pwd(char **args);
int		ft_unset(char **arg, t_env *env_set);
int		ft_export(char **arg, t_env *env_set);

//		error.c
void	print_error(char *msg);
void	exit_error(char *msg, int code);
int		error(char *msg, int code);
void	*error_null(char *msg);
void	error_manag(int ret);

//		exec.c
int		ft_cmd(char **args, t_env *env);
int		exec_block(t_cmd *to_exec, t_env *env, int mod);
int		execute_cmd(char **args, t_env *env, int mod);
int		exec_in_main(t_cmd *cmd, t_env *env, int mod);
int		exec_in_child(char **args, t_env *env, int mod);

//		expand_ev.c
void	expand_word(char **word, char **env);
char	**expand_args(char **args, char **env);
int		redir_expanded_is_valid(char *word_expanded);
int		expand_redir(t_list *redirs, char **env);
int		expand_ev(t_list *cmds, t_env *env);

//		expand_utils.c
char	*get_ev_name(char *str);
char	*get_ev_value(char *ev_name, char **env);
void	rearrange_word(char **word, int *i, char **env);
void	delete_quotes(char **word);

//		free.c
void	free_token(void *ptr);
void	free_redir(void *ptr);
void	free_cmd(void *ptr);

//		ft_strtok.c
char	**ft_strtok(char *str, char *delim);

//		get_cmds.c
t_list	*get_cmds(t_list *tokens);

//		get_tokens.c
t_list	*get_tokens(char *line);
int		get_len_word(char *line);

//		main.c
t_env	*env_manag(char **env, t_env *to_free, int mod);
void	wait_this_fk_process(t_env *env);
int		minishell(t_env *env_set);
int		ft_exit(char **args, int print_exit, t_env *env_set);
int		main(int ac, char **av, char **env);

//		parse.c
t_list	*parse(char *line);
char	*parse_cmd(char **path, char **cmd);
void	cette_fct_sert_pour_la_norm_pose_pas_de_question(t_env *env, const int mod, int ret);

//		redir.c
int		redir_heredoc(char *stop);
char	**here_doc(char *stop);
int		redir_to_stdout(void *file, int mod);
int		redir_to_stdin(void *file);
void	ft_pipex(t_cmd *cmd, t_env *env);

// 		signals.c                                       
void	sigint_handler(int signum);
void	sig_hd_handler(int signum);
void	set_sig(int signum, void (*handler)(int));

//		strs.c
void	free_strs(char **strs);
char	**copy_strs(char **strs);
int		strs_len(char **strs);
char	**strs_append(char **strs, const char *str);
char	**strs_join(char **strs1, char **strs2);

//		tty.c
void	set_tty(void);
void	reset_tty(void);

//		utils.c.c
char	*str_insert(char *dst, const char *src, size_t n);
char	*get_str_truncated(const char *str, int start, int len);
char	*trunc_str(char *str, int start, int len);
int		get_token_type(t_token *tok);
char	*get_token_value(t_token *tok);

//		utils_2.c
int		is_symbol(char c); 
int		is_whitespace(char c);
void	skip_whitespace(char **line);
void	my_strncpy(char *dst, const char *src, int n);
int		my_strcmp(char *s1, char *s2);

#endif
