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

CGI_DIR = ./cgi/
UPLOAD_CGI_SRC = cgi_upload_clk.cpp
UPLOAD_CGI_EXE = cgi_upload_clk
SESSION_CGI_SRC = cgi_session.cpp
SESSION_CGI_EXE = cgi_session

CXX = c++
CXXFLAGS = $(MYFLAGS) $(INCLUDE)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME);
	$(CXX) $(MYFLAGS) $(CGI_DIR)$(UPLOAD_CGI_SRC) -o $(CGI_DIR)$(UPLOAD_CGI_EXE);
	$(CXX) $(MYFLAGS) $(CGI_DIR)$(SESSION_CGI_SRC) -o $(CGI_DIR)$(SESSION_CGI_EXE);

clean :
	rm -rf $(OBJS);

fclean : clean
	rm -rf $(CGI_DIR)$(UPLOAD_CGI_EXE);
	rm -rf $(CGI_DIR)$(SESSION_CGI_EXE);
	rm -rf $(NAME);

re :
	make fclean;
	make all;
