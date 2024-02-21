#include "Http.hpp"

Http::Http() : autoindex_(true), \
                    root_("./www"), \
                    client_max_body_size_(10000)
{
  // std::vector<Server> server_block_;
  // std::map<int, std::string> error_page_;
}

Http& Http::operator=(const Http& other)
{
	if (this != &other)
        ;
	return (*this);
}

Http::Http(const Http &other)
{
	*this = other;
}

Http::~Http()
{
}