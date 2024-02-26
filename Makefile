SRCS = main.cpp \
		./srcs/Directive/Http.cpp \
		./srcs/Directive/Location.cpp \
		./srcs/Directive/Server.cpp

OBJS = $(SRCS:.cpp=.o)
INCLUDE = -I ./include/Directive
NAME = webserv
CXX = c++
MYFLAGS = -std=c++98 -Wall -Wextra -Werror
CXXFLAGS = $(MYFLAGS) $(INCLUDE)


all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME);

clean :
		rm -f $(OBJS);

fclean :
		rm -f $(OBJS);
		rm -f $(NAME);

re :
		make fclean;
		make all;
