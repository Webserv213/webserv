#include "Location.hpp"

Location::Location() : operation_("~"), \
                    root_("./www"), \
                    autoindex_(true), \
                    client_max_body_size_(10000)
{
    // path_postfix_;
    // error_page_;
    // location_block_;
}

Location& Location::operator=(const Location& other)
{
	if (this != &other)
        ;
	return (*this);
}

Location::Location(const Location &other)
{
	*this = other;
}

Location::~Location()
{
}