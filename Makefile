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

NAME = webserv

INCLUDE = -I ./include/Directive -I ./include/HttpMessage -I ./include
MYFLAGS = -std=c++98 -Wall -Wextra -Werror

UPLOAD_CGI_DIR = ./CLK/cgi/
UPLOAD_CGI_SRC = upload.cpp
UPLOAD_CGI_EXE = upload.clk

CXX = c++
CXXFLAGS = $(MYFLAGS) $(INCLUDE)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME);
	$(CXX) $(MYFLAGS) $(UPLOAD_CGI_DIR)$(UPLOAD_CGI_SRC) -o $(UPLOAD_CGI_DIR)$(UPLOAD_CGI_EXE);

clean :
	rm -rf $(OBJS);

fclean : clean
	rm -rf $(UPLOAD_CGI_DIR)/$(UPLOAD_CGI_EXE);
	rm -rf $(NAME);

re :
	make fclean;
	make all;
