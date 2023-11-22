/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:22:21 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:22:23 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include "get_next_line.h"

# define ERR_NO_OPT "option not supported"
# define ERR_NO_ARG "arguments not supported"
# define ERR_PARSE "parse error"
# define ERR_BAD_SUB "bad substitution"
# define ERR_INTERNAL "internal error"
# define ERR_CMD_NOT_FOUND "command not found"
# define ERR_MULTI_LINE "multiline commands not supported"
# define ERR_AMBIGUOUS_REDIRECT "ambiguous redirect"
# define ENV_OPT "*@#-$!0_"

/*
**	used in exec_process, according bits in exec will be set
**	to tell wait_and_signal_check to wait or ignore a process
*/

enum	e_process_info
{
	PROCESS_BUILT_IN = 1,
	PROCESS_NOT_FOUND = 2,
	PROCESS_FORKED = 4,
	PROCESS_PIPED = 8,
	PROCESS_NOT_EXEC = 16
};

enum	e_parse
{
	LITERAL_TOKEN = 0,
	INTERPRETED_TOKEN = 1,
	LITERAL_PIPE = 1,
	LAST_STATUS = 1,
	BRACKET_SUB = 2,
	LITERAL_BACKSLASH = 2,
	LITERAL_REDIR = 2,
	META = 2,
	CMD = 4,
	LITERAL_SEMICOLON = 4,
	LITERAL_QUOTE = 4,
	EXPANDED_TOKEN = 4,
	LITERAL_WHITESPACE = 8,
	LITERAL_ENV = 8,
	ARG = 8,
	QUOTED_SINGLE = 16,
	QUOTED_DOUBLE = 32,
	QUOTED = 64,
	LEXER = 128
};

enum	e_filedes
{
	IN,
	OUT
};

enum	e_meta
{
	REDIR_OUTPUT,
	REDIR_OUTPUT_APPEND,
	REDIR_INPUT,
	SEMICOLON,
	PIPE,
	NOT_ASSIGNED
};

enum	e_status
{
	STOP = -4,
	SHELL_ERROR = -3,
	BAD_SUBSTITUTION = -2,
	INTERNAL_ERROR = -1,
	SUCCESS = 0,
	ERROR = 1,
	PARSING_ERROR = 2,
	NO_OPT = 3,
	NO_ARG = 4,
	NO_MULTI_LINE = 5,
	BACKSLASH_TERM = 6,
	AMBIGUOUS_REDIRECT = 8,
	IGN_ENV_SUB = 16,
	EXEC_FAILURE = 126,
	CMD_NOT_FOUND = 127,
	TERM_C = 130
};

typedef struct		s_list
{
	char			*name;
	int				value;
	struct s_list	*next;
	char			token_type;
}					t_list;

typedef	struct		s_cmd
{
	char			*name;
	char			token_type;
	t_list			*args;
	t_list			**current_arg;
	t_list			*in_files;
	t_list			**current_in_file;
	t_list			*out_files;
	t_list			**current_out_file;
	char			**exec_args;
	int				n_args;
	int				in;
	int				out;
	int				separator;
	char			exec;
	pid_t			pid;
	struct s_cmd	*next;
}					t_cmd;

typedef	struct		s_shell
{
	char			**envp_cpy;
	char			**tokens;
	char			*tokens_info;
	unsigned char	parse;
	unsigned char	parse_env;
	int				process_level;
	int				current_status;
	int				status;
	int				current_n_tokens;
	unsigned int	n_env;
	unsigned int	envp_cpy_size;
	char			*home_parent_shell;
	size_t			home_parent_shell_len;
	char			*saved_pwd;
	t_cmd			*commands;
	t_cmd			**current_command;
	int				filedes[2];
	char			home_env_name[5];
	char			path_env_name[5];
	char			*absolute_path;
	char			(*built_in_str[8]);
	void			(*built_in_fun[8])(struct s_shell **);
	char			(*meta_str[7]);
	struct stat		statbuf;
}					t_shell;

/*
**	srcs/shell.c
*/

void				run_shell(t_shell **shell);
void				prompt(void);
void				update_status(t_shell **shell);

/*
**	srcs/built_in/
*/

void				built_in_cd(t_shell **shell);
int					built_in_cd_save_and_chdir(t_shell **shell, char *dir,
						char *oldpwd);
void				built_in_echo(t_shell **shell);
char				built_in_echo_has_opt(char *line);
void				built_in_env(t_shell **shell);
void				built_in_exit(t_shell **shell);
t_list				*built_in_exit_fetch_arg(t_shell **shell);
bool				built_in_exit_within_range(t_shell **shell,
						char *exit_arg, int *exit_status);
void				built_in_export(t_shell **shell);
void				built_in_export_no_arg(t_shell **shell);
void				built_in_export_print(char *envp);
int					built_in_export_with_arg(t_shell **shell, char *arg,
						int internal);
bool				built_in_export_valid_identifier(const char *arg);
void				built_in_pwd(t_shell **shell);
void				built_in_unset(t_shell **shell);

/*
**	srcs/exec/check_path.c
*/

void				check_exec_relative_path(t_shell **shell);
void				check_exec_absolute_path(t_shell **shell);

/*
**	srcs/exec/exec_cmd_line.c
*/

void				exec_cmd_line(t_shell **shell);
void				save_std(t_shell **shell, int *saved_std);
void				exec_process(t_shell **shell, int *tmp_fd);
void				reset_std(t_shell **shell, int *saved_std);

/*
**	srcs/exec/execute.c
*/

bool				execute_builtin(t_shell **shell, int *tmp_fd);
void				exec_call_builtin(t_shell **shell, int index, int *tmp_fd);
void				exec_call(t_shell **shell, int *fd);
void				init_exec_args(t_shell **shell);

/*
**	srcs/exec/handle_separator.c
*/

void				handle_pipe(t_shell **shell, t_list **current,
						int *tmp_fd);
int					handle_semicolon(t_shell **shell, t_cmd **current,
						t_cmd **wait_start);

/*
**	srcs/exec/process_io.c
*/

int					process_in(t_shell **shell, int *tmp_fd, int *saved_std);
int					process_out(t_shell **shell, int *tmp_fd, int *saved_std);

/*
**	srcs/exec/var_arg_exp.c
*/

void				cmd_args_var_expansion(t_shell **shell);
int					cmd_args_expand_tokens(t_shell **shell, t_list ***tmp);
void				cmd_args_expand_literal(t_shell **shell,
						char ***literal_result, char **expand, t_list **tmp);
int					cmd_args_update_args_lit(t_shell **shell, t_list ***tmp,
						char **expand, char ***lit_tokens);
int					cmd_args_get_int_tokens(t_shell **shell,
						char ***lit_tokens, char ***int_tokens);

/*
**	srcs/exec/var_name_exp.c
*/

void				cmd_name_var_expansion(t_shell **shell);
void				cmd_name_expand_tokens(t_shell **shell, char ***lit_result,
						char ***int_result, char **expand);
void				cmd_name_shift(t_shell **shell, char **tokens,
						char **expand);
void				cmd_name_update_args(t_shell **shell, t_list *next);
void				cmd_name_update(t_shell **shell,
						char **interpreted_result);

/*
**	srcs/exec/var_file_exp.c
*/

int					cmd_io_files_var_expansion(t_shell **shell, t_list *file);
int					cmd_io_files_expand_tokens(t_shell **shell, t_list *file);

/*
**	srcs/exec/variable_expansion.c
*/

int					variable_expansion(t_shell **shell);
int					bad_redirect(t_shell **shell, char **expand,
						char ***lit_tokens, char ***int_result);
int					free_tmp_tokens(char **expand, char ***literal_result,
						char ***interpreted_result);

/*
**	srcs/exec/var_name_exp.c
*/

void				cmd_name_var_expansion(t_shell **shell);
void				cmd_name_expand_tokens(t_shell **shell, char ***lit_result,
						char ***int_result, char **expand);
void				cmd_name_shift(t_shell **shell, char **tokens,
						char **expand);
void				cmd_name_update_args(t_shell **shell, t_list *next);
void				cmd_name_update(t_shell **shell,
						char **interpreted_result);

/*
**	srcs/exec/var_file_exp.c
*/

int					cmd_io_files_var_expansion(t_shell **shell, t_list *file);
int					cmd_io_files_expand_tokens(t_shell **shell, t_list *file);

/*
**	srcs/exec/variable_expansion.c
*/

int					variable_expansion(t_shell **shell);
int					bad_redirect(t_shell **shell, char **expand,
						char ***lit_tokens, char ***int_result);
int					free_tmp_tokens(char **expand, char ***literal_result,
						char ***interpreted_result);

/*
**	srcs/lexer/lexer.c
*/

void				lexer(t_shell **shell, char **input);

/*
**	srcs/lexer/handle_env/env_checks.c
*/

char				*fetch_env(char *line, t_shell **shell, char *free_env);
char				*check_env_literal(t_shell **shell, char *line, int *len,
						char brackets);
char				*check_env_str(char *line, int len, char **envp,
						int brackets);
void				check_return_value(char **env_str, char *free_env,
						int return_value);

/*
**	srcs/lexer/handle_env/env_flags.c
*/

void				set_env_sub_error(t_shell **shell,
						char **new_literal_token, int *i,
						unsigned char bracket_sub);
unsigned char		flag_offset(unsigned char *parse, char **new_literal_token,
						int index, int *i);
void				handle_bslash_input(t_shell **shell,
						char **new_literal_token, int i, unsigned char *parse);

/*
**	srcs/lexer/handle_env/env_home.c
*/

bool				integrate_home(char **input, int *i, t_shell **shell,
						unsigned char *parse);
void				handle_tilde(char **input, int *i, t_shell **shell,
						unsigned char *parse);

/*
**	srcs/lexer/handle_env/env_offset.c
*/

unsigned char		get_offset_brackets(t_shell **shell, int *i, int index);
bool				get_offset_no_opt(t_shell **shell,
						char **new_literal_token, int *i);
void				get_offset_ignore_env(t_shell **shell,
						char **new_literal_token, int *i, int index);
unsigned char		get_offset(t_shell **shell, char **new_literal_token,
						int index, int *i);

/*
**	srcs/lexer/handle_env/env_offset2.c
*/

int					get_offset_is_option(char **lit_token, int *i, int index);
void				get_offset_skip_option(t_shell **shell, char **lit_token,
						int index, int *i);
void				get_offset_stop_parsing(t_shell **shell, char **lit_token,
						int index, int *i);
unsigned char		get_offset_end(t_shell **shell);

/*
**	srcs/lexer/handle_env/env_strcat.c
*/

char				*create_lh_string(t_shell **shell, char **input,
						unsigned char *parse, int *home_len);
char				*concat_result(char *env_str, char *lit_token, char swap,
						char free_env);
char				*create_temp_line(char *literal_token, int *i, int index,
						t_shell **shell);
void				update_input(char **input, char **lh_string,
						char **rh_string, int size);

/*
**	srcs/lexer/handle_env/handle_env.c
*/

bool				escape_characters(t_shell **shell, char **env_str,
						unsigned char *parse);
void				handle_env_lexer_level(t_shell **shell, char **input,
						int *i);
void				handle_env_exec_level(t_shell **shell, char **input,
						int *i, unsigned char *parse);
int					handle_env(t_shell **shell, char **input, int *i,
						unsigned char *parse);

/*
**	srcs/lexer/input/escape_characters.c
*/

void				count_esc_char(char **env_str, int *to_escape,
						int *len, unsigned char *parse);
void				expand_esc_char(char **env_str, char **new_env_str,
						int *new_len, unsigned char *parse);
bool				escape_characters(t_shell **shell, char **env_str,
						unsigned char *parse);

/*
**	srcs/lexer/input/input_to_literal_input.c
*/

void				integrate_env(char **input, int *i, t_shell **shell);
void				reset_literals(char **input, int i, unsigned char *parse);
int					parse_literal_input(t_shell **shell, char **input,
						int *i, unsigned char *parse);
void				input_to_literal_input(t_shell **shell, char **input,
						int lexer);

/*
**	srcs/lexer/input/read_input.c
*/

int					new_input(t_shell **shell, char **input, char **saved,
						char **new);
void				save_input(t_shell **shell, char **input, char **saved,
						char **new);
void				handle_eof(t_shell **shell, char **input, int ret);
void				read_line(t_shell **shell, char **input);

/*
**	srcs/lexer/input/trim_input.c
*/

void				trim_words_update_parse_len(unsigned char *parse, int *len,
						char *line);
bool				trim_words_is_meta(char c, unsigned char parse);
int					trim_words(char *line);
void				trim_quotes(char *line, int *i, unsigned char parse);
void				trim_brackets(char *line, int *i);

/*
**	srcs/lexer/tokens/interpreted_tokens_flags.c
*/

void				set_literal_interpreted(char *line, unsigned char *parse);
void				reset_parse_bslash(char **new_literal_token, int i,
						unsigned char *parse);
void				reset_parse_literal_env(char **new_literal_token, int i,
						unsigned char *parse);
int					handle_quotes(char **new_literal_token, int i,
						unsigned char *parse, bool trim);
void				handle_bslash(t_shell **shell, char **new_literal_token,
						int i, unsigned char *parse);

/*
**	srcs/lexer/tokens/literal_input_to_literal_tokens.c
*/

int					parse_meta_token(char *line, int i);
void				set_literal(char *line, unsigned char *parse);
char				**create_literal_tokens(char **input, t_shell **shell);
void				fill_tokens(char **literal_tokens, char *line,
						t_shell **shell);
char				*new_literal_token(t_shell **shell, int len, char *token);

/*
**	srcs/lexer/tokens/literal_tokens_flags.c
*/

void				flag_literal_bslash(unsigned char *parse,
						unsigned char *delete);
void				flag_literal_env(unsigned char *parse,
						unsigned char *delete);
void				flag_bslash_term(t_shell **shell, unsigned char *delete);
void				flag_literal_quote(unsigned char *parse,
						unsigned char *delete, char to_escape);
void				flag_quotes(int n_quotes, t_shell **shell);

/*
**	srcs/lexer/tokens/literal_tokens_to_interpreted_tokens.c
*/

char				**create_interpreted_tokens(char **literal_tokens,
						int tokens, t_shell **shell, int lexer);
char				*new_interpreted_token(char **literal_token,
						t_shell **shell, int lexer);
char				*final_interpreted_token(int prev_size, int new_size,
						char *literal_token_copy);
void				literal_to_interpreted(char **new_literal_token,
						t_shell **shell, int lexer);
int					calculate_new_size(char *line, int prev_size);

/*
**	parser/parser.c
*/

void				parser(t_shell **shell);
int					init_cmd(t_shell **shell, char *cmd_name, int *i);
int					parse_tokens(t_shell **shell, t_cmd **current, int *i);
int					assign_io(t_shell **shell, t_cmd **current, int meta,
						int *i);
int					check_if_separator(t_shell **shell, t_cmd **current,
						int *i, int meta);

/*
**	srcs/utils/array_utils.c
*/

char				**copy_array(t_shell **shell, char **src, int size);
void				free_array(char **array);

/*
**	srcs/utils/cd_dir_utils.c
*/

int					get_fetch_dir(t_shell **shell, t_list *arg,
						char **fetch_dir, bool *previous);
void				get_new_old_pwd(t_shell **shell, char *tmp,
						char **new_oldpwd, char *new_path);
char				*built_in_cd_fetch_oldpwd(t_shell **shell);
char				*fetch_pwd(t_shell **shell);
char				*built_in_cd_get_dir(t_shell **shell, t_list *arg,
						bool *previous);

/*
**	srcs/utils/cd_env_utils.c
*/

int					built_in_cd_home(t_shell **shell, char *dir);
int					built_in_cd_previous(t_shell **shell, char *dir);
void				update_pwd(t_shell **shell, char *new_path,
						size_t new_path_len);

/*
**	srcs/utils/cd_path_utils.c
*/

void				delete_current_path(char *new_path, size_t new_path_len,
						size_t current_path, size_t i);
void				shift_path(char *new_path, size_t new_path_len,
						size_t current_path, size_t i);
void				shift_path_dot(char *new_path, size_t new_path_len,
						size_t index);
void				join_path(char *new_path, size_t new_path_len);
void				slash_sub(char *new_path, size_t new_path_len);

/*
**	srcs/utils/cd_path_utils2.c
*/

void				cd_update_current_path(char *new_path,
						size_t *current_path);
void				update_index(char *new_path, size_t new_path_len,
						size_t current_path, size_t *i);
void				delete_shift_slash(char *new_path, size_t index,
						size_t new_path_len);
void				remove_dot_component(char *new_path, size_t new_path_len);
void				remove_dot_dot_component(char *new_path,
						size_t new_path_len);

/*
**	srcs/utils/cd_path_utils3.c
*/

char				*create_new_path(t_shell **shell, char *dir);
char				*concat_new_path(t_shell **shell, char *pwd, char *dir,
						size_t dir_len);
char				*dot_sub(char *new_path, size_t new_path_len);
char				*create_sub_path(char *new_path, size_t len);

/*
**	srcs/utils/cd_utils.c
*/

int					valid_sub(char *current_path);
size_t				get_new_len(char *new_path, size_t new_path_len);
int					is_relative_dot(char *dir);
int					is_dot_dir(char *dir);

/*
**	srcs/utils/cd_utils2.c
*/

int					free_paths(char **new_path, char **new_path_tmp,
						char **dot_sub_path, char *dir);
char				*create_tmp_path(t_shell **shell, char **new_path,
						size_t new_path_len, char *dir);

/*
**	srcs/utils/command_utils.c
*/

t_cmd				*new_command(t_shell **shell, char *name);
void				push_command(t_shell **shell, char *cmd_name, int token);
void				free_commands(t_shell **shell);

/*
**	srcs/utils/env_utils.c
*/

int					n_env(char **envp);
int					match_env(char *current, char *to_find);
int					change_env(t_shell **shell, char **envp, char *name,
						int internal);

/*
**	srcs/utils/error_utils.c
*/

void				lexer_error_handler(t_shell **shell, char *args,
						int error_code, char *error_message);
void				error_handler(t_shell **shell, char *args, int error_code,
						char *error_message);
void				error_printer(char *command, char *args,
						char *error_message);

/*
**	srcs/utils/exec_utils.c
*/

void				dup2_and_close(t_shell **shell, int fd, int stdio);
int					open_file(t_shell **shell, t_list **current, int *fd,
						int mode);

/*
**	srcs/utils/export_utils.c
*/

int					built_in_export_escape_char(char *envp);
void				built_in_export_display_env(char *envp);

/*
**	srcs/utils/ft_atoi.c
*/

int					ft_atoi(const char *str);

/*
**	srcs/utils/ft_itoa_base.c
*/

char				*ft_itoa_base(int value, int base);

/*
**	srcs/utils/input_to_literal_tokens_utils.c
*/

int					n_tokens(char *line);
bool				is_escaped_meta_token(char *token);

/*
**	srcs/utils/list_utils.c
*/

t_list				*new_node(t_shell **shell, char *name, int value);
t_list				**last_node(t_list **list);
void				push_to_list(t_list **location, t_list *new);
void				free_list(t_list *list);
void				free_all_lists(t_cmd **command);

/*
**	srcs/utils/list_utils2.c
*/

void				update_current(t_list ***current, t_list **node);
void				update_node(t_shell **shell, t_list **node, char *token);
void				update_list(t_shell **shell, t_list **list, char *token,
						t_list ***current);

/*
**	srcs/utils/literal_tokens_to_interpreted_tokens_utils.c
*/

bool				detect_ignored_env(t_shell **shell,
						char **new_literal_token, int i, unsigned char parse);
char				*free_lit_copy(char **literal_token_copy);
void				copy_int_token(char **result, char *literal_token_copy,
						int prev_size);
unsigned char		bslash_term(t_shell **shell, char **literal_tokens,
						char **final_tokens, int token);
void				ignore_env_substitution(t_shell **shell, char **lit_tokens,
						char **int_tokens, int n);

/*
**	srcs/utils/parse_utils.c
*/

bool				is_meta_token(t_shell **shell, int token);
int					meta_to_code(char *token);
void				push_cmd_arg(t_shell **shell, char *arg, int *i);
void				push_io_file(t_shell **shell, int meta, char *file_name,
						int *i);

/*
**	srcs/utils/push_pop_env.c
*/

void				pop_env(t_shell **shell, char *name);
int					push_env(t_shell **shell, char *name, int internal);
int					realloc_env(t_shell **shell, char *name, int internal);
void				free_if_failed(t_shell **shell, char *str, char **temp);

/*
**	srcs/utils/path_utils.c
*/

bool				is_path(char *cmd_name);
char				*check_exec_copy_path(t_shell **shell);
void				check_exec_free(t_shell **shell, char **full_path,
						char **exec_name);
char				*check_exec_get_exec_name(t_shell **shell,
						char **full_path);
char				update_current_path(t_shell **shell, char **full_path,
						char **exec_name, int *i);

/*
**	srcs/utils/shell_utils.c
*/

void				assign_pwd(t_shell **shell);
t_shell				*new_shell(char **envp);
void				assign_home_parent_shell(t_shell **new,
						char **home_parent_shell);
void				assign_built_in(t_shell **shell);
void				reset_shell(t_shell **shell);

/*
**	srcs/utils/signal_utils.c
*/

void				shell_signals(t_shell **shell);
void				shell_signal_handler(int signal);
void				exec_signals(t_shell **shell);
void				exec_signal_handler(int signal);
void				wait_and_signal_check(t_shell **shell, t_cmd *current);

/*
**	srcs/utils/sort.c
*/

void				sort_env(char **array, int start, int length);

/*
**	srcs/utils/utils1.c
*/

void				ft_bzero(void *s, size_t n);
int					ft_isalnum(int c);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				indexing(char **line, char **valid_escape_char,
						int *index);
void				iter_whitespaces(char **line, int *index);

/*
**	rcs/utils/utils2.c
*/

void				ft_putstr_fd(char *s, int fd);
char				*ft_strchr(const char *s, int c);
int					ft_strcmp(void *s1, void *s2);
int					match_str(char *s1, char *s2, char full);
size_t				str_len(char *str, char *stop, char add);

/*
**	srcs/utils/var_arg_utils.c
*/

void				cmd_args_loop_expand(t_shell **shell, t_list **tmp);
void				cmd_args_update_current_arg_int(t_shell **shell,
						t_list ***tmp);
void				cmd_args_push_arg_list(t_shell **shell,
						char **interpreted_result, t_list ***tmp);
void				cmd_args_sync_name(t_shell **shell, t_list ***tmp,
						char *name);
void				cmd_args_push_update(t_list **list, t_list **end,
						t_list ***tmp);

/*
**	srcs/utils/var_name_utils.c
*/

void				cmd_name_update_name(t_shell **shell,
						char **interpreted_result);
void				cmd_name_push_arg_list(t_shell **shell,
						char **interpreted_result);
char				*cmd_name_cpy_name(t_shell **shell);

#endif
