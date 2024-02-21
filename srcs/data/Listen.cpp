#include "Listen.hpp"

Listen::Listen() : ip_("127.0.0.1"), \
                    port_(8080)
{
}

Listen& Listen::operator=(const Listen& other)
{
	if (this != &other)
        ;
	return (*this);
}

Listen::Listen(const Listen &other)
{
	*this = other;
}

Listen::~Listen()
{
}