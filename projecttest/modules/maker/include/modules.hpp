#pragma once

int add(int a, int b);
int sub(int a, int b);

class Animal
{
public:
	int a, b;

	Animal();

	void makeNull();
	void set(int a, int b);
};