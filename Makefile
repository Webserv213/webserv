SRCS = main.cpp \
		./srcs/data/Http.cpp \
		./srcs/data/Listen.cpp \
		./srcs/data/Location.cpp \
		./srcs/data/Server.cpp

OBJS = $(SRCS:.cpp=.o)
INCLUDE = -I ./include/data
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
