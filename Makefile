SRCS = main.cpp \
		./srcs/Directive/Http.cpp \
		./srcs/Directive/Location.cpp \
		./srcs/Directive/Server.cpp \
		./srcs/HttpMessage/ReqHeaders.cpp \
		./srcs/HttpMessage/Request.cpp \
		./srcs/HttpMessage/RequestLine.cpp \
		./srcs/HttpMessage/ResHeaders.cpp \
		./srcs/HttpMessage/Response.cpp \
		./srcs/HttpMessage/ResponseLine.cpp \
		./srcs/EventRecorder.cpp \
		./srcs/KeventHandler.cpp \
		./srcs/Utils.cpp

OBJS = $(SRCS:.cpp=.o)
INCLUDE = -I ./include/Directive -I ./include/HttpMessage -I ./include
NAME = webserv
CXX = c++
MYFLAGS = -std=c++98 -Wall -Wextra -Werror -g -fsanitize=address
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
