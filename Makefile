# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mjiam <mjiam@student.codam.nl>               +#+                      #
#                                                    +#+                       #
#    Created: 2020/11/09 19:40:29 by mjiam         #+#    #+#                  #
#    Updated: 2020/11/09 20:41:45 by mjiam         ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

FLAGS		=	-Wall -Wextra -Werror

INCLUDES	=	-Iincludes

SRCS		=	srcs/main.c\
				srcs/shell.c \
				srcs/built_in/cd/cd.c \
				srcs/built_in/echo/echo.c \
				srcs/built_in/env/env.c \
				srcs/built_in/exit/exit.c \
				srcs/built_in/export/export.c \
				srcs/built_in/pwd/pwd.c \
				srcs/built_in/unset/unset.c \
				srcs/exec/check_path.c \
				srcs/exec/exec_cmd_line.c \
				srcs/exec/execute.c \
				srcs/exec/handle_separator.c \
				srcs/exec/process_io.c \
				srcs/exec/var_arg_exp.c \
				srcs/exec/var_file_exp.c \
				srcs/exec/var_name_exp.c \
				srcs/exec/variable_expansion.c \
				srcs/gnl/get_next_line.c \
				srcs/gnl/get_next_line_utils.c \
				srcs/lexer/lexer.c \
				srcs/lexer/handle_env/env_checks.c \
				srcs/lexer/handle_env/env_flags.c \
				srcs/lexer/handle_env/env_home.c \
				srcs/lexer/handle_env/env_offset.c \
				srcs/lexer/handle_env/env_offset2.c \
				srcs/lexer/handle_env/env_strcat.c \
				srcs/lexer/handle_env/handle_env.c \
				srcs/lexer/input/escape_characters.c \
				srcs/lexer/input/input_to_literal_input.c \
				srcs/lexer/input/read_input.c \
				srcs/lexer/input/trim_input.c \
				srcs/lexer/tokens/interpreted_tokens_flags.c \
				srcs/lexer/tokens/literal_input_to_literal_tokens.c \
				srcs/lexer/tokens/literal_tokens_flags.c \
				srcs/lexer/tokens/literal_tokens_to_interpreted_tokens.c \
				srcs/parser/parser.c \
				srcs/utils/array_utils.c \
				srcs/utils/command_utils.c \
				srcs/utils/cd_dir_utils.c \
				srcs/utils/cd_env_utils.c \
				srcs/utils/cd_path_utils.c \
				srcs/utils/cd_path_utils2.c \
				srcs/utils/cd_path_utils3.c \
				srcs/utils/cd_utils.c \
				srcs/utils/cd_utils2.c \
				srcs/utils/env_utils.c \
				srcs/utils/error_utils.c \
				srcs/utils/exec_utils.c \
				srcs/utils/export_utils.c \
				srcs/utils/ft_atoi.c \
				srcs/utils/ft_itoa_base.c \
				srcs/utils/input_to_literal_tokens_utils.c \
				srcs/utils/literal_tokens_to_interpreted_tokens_utils.c \
				srcs/utils/list_utils.c \
				srcs/utils/list_utils2.c \
				srcs/utils/path_utils.c \
				srcs/utils/parse_utils.c \
				srcs/utils/push_pop_env.c \
				srcs/utils/shell_utils.c \
				srcs/utils/signal_utils.c \
				srcs/utils/sort.c \
				srcs/utils/utils1.c \
				srcs/utils/utils2.c \
				srcs/utils/var_arg_utils.c \
				srcs/utils/var_name_utils.c \

OBJ			=	$(SRCS:.c=.o)

CYAN		= 	\033[38;5;51m
WHITE		= 	\033[38;5;15m

all: $(NAME)

$(NAME): $(OBJ)
	@gcc $(FLAGS) $(OBJ) -g -o $(NAME)
	@chmod 755 ./$(NAME)
	@echo "$(CYAN)$(NAME) compiled$(WHITE)"


%.o: %.c $(HEADER)
	@gcc $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "$(CYAN)Object files cleaned out$(WHITE)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)Executable cleaned out$(WHITE)"

re:
	@echo "$(CYAN)Remaking$(WHITE)"
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
