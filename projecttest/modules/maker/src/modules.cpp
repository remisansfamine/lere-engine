#include "modules.hpp"

int add(int a, int b)
{
	return a + b;
}

int sub(int a, int b)
{
	return a - b;
}

Animal::Animal()
{
	a = 3;
	b = 7;
}

void Animal::makeNull()
{
	a = 0;
	b = 0;
}

void Animal::set(int a, int b)
{
	this->a = a;
	this->b = b;
}