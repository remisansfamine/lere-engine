#pragma once

#include <Python.h>

class CPyInstance
{
public:
	CPyInstance()
	{
		Py_Initialize();
	}

	~CPyInstance()
	{
		Py_Finalize();
	}

	void reload()
	{
		Py_Finalize();
		Py_Initialize();
	}
};


class CPyObject
{
private:
	PyObject* p = nullptr;

public:
	CPyObject() = default;

	CPyObject(PyObject* pp)
	{
		if (pp)
			Py_INCREF(pp);

		p = pp;
	}

	CPyObject(CPyObject& other) : p(other.addRef())
	{ }

	~CPyObject()
	{
		release();
	}

	PyObject* get()
	{
		return p;
	}

	operator PyObject* ()
	{
		return p;
	}

	PyObject* setObject(PyObject* _p)
	{
		if (p)
			release();

		return (p = _p);
	}

	PyObject* addRef()
	{
		if (p)
			Py_INCREF(p);

		return p;
	}

	void release()
	{
		if (p)
			Py_DECREF(p);

		p = nullptr;
	}

	PyObject* operator->()
	{
		return p;
	}

	PyObject* operator=(PyObject* pp)
	{
		release();

		if (pp)
			Py_INCREF(pp);

		p = pp;

		return p;
	}

	double asDouble() { return p ? PyFloat_AsDouble(p) : 0.f; }

	float asFloat() { return  (float)asDouble(); }

	long asLong() { return p ? PyLong_AsLong(p) : 0; }

	int asInt() { return (int)asLong(); }
};
