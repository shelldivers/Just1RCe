SRCS_MANDATORY = srcs/main.cc 
SRCS_MANDATORY += srcs/channel/channel_invite_list.cc srcs/channel/channel_mode.cc srcs/channel/channel.cc 
SRCS_MANDATORY += srcs/client/Client_auth.cc srcs/client/client_get_messages.cc srcs/client/client_mode.cc srcs/client/client_network_info.cc srcs/client/client_send_message.cc srcs/client/client.cc 
SRCS_MANDATORY += srcs/command_mapping/command_mapping.cc 
SRCS_MANDATORY += srcs/commands/nick_command_handler.cc srcs/commands/pass_command_handler.cc srcs/commands/user_command_handler.cc srcs/commands/invite_command_handler.cc srcs/commands/join_command_handler.cc srcs/commands/kick_command_handler.cc srcs/commands/mode_command_handler.cc srcs/commands/part_command_handler.cc srcs/commands/ping_command_handler.cc srcs/commands/privmsg_command_handler.cc srcs/commands/quit_command_handler.cc srcs/commands/topic_command_handler.cc 
SRCS_MANDATORY += srcs/commands/modes/mode_i.cc srcs/commands/modes/mode_k.cc srcs/commands/modes/mode_l.cc srcs/commands/modes/mode_o.cc srcs/commands/modes/mode_t.cc
SRCS_MANDATORY += srcs/context_holder/context_holder.cc 
SRCS_MANDATORY += srcs/in_memory_db_context/channel_table.cc srcs/in_memory_db_context/client_mode_table.cc srcs/in_memory_db_context/client_table.cc srcs/in_memory_db_context/in_memory_db_context.cc srcs/in_memory_db_context/mapping_table.cc 
SRCS_MANDATORY += srcs/parse/parser.cc 
SRCS_MANDATORY += srcs/responser/response_generator.cc 
SRCS_MANDATORY += srcs/service/event_handler.cc srcs/service/event_utility.cc srcs/service/run_irc_server.cc 
SRCS_MANDATORY += srcs/socket/tcp_socket.cc

INCLUDES = includes

INCLUDES_MANDATORY := $(addprefix -I, $(INCLUDES_MANDATORY))

OBJS_MANDATORY = $(SRCS_MANDATORY:.cc=.o)

NAME = Just1RCe
CC = g++
FLAG = -Wall -Wextra -Werror -std=c++98

all : $(NAME)

$(NAME) : $(OBJS_MANDATORY) 
	$(CC) $(FLAG) -o $(NAME) $(OBJS_MANDATORY) $(INCLUDES_MANDATORY) 

%.o : %.cc
	$(CC) $(FLAG) -c $< -o $@ $(INCLUDES_MANDATORY)

clean :
	@rm -f $(OBJS_MANDATORY)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re