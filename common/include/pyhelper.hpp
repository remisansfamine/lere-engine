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
};


class CPyObject
{
private:
	PyObject* p = nullptr;

public:
	CPyObject() = default;

	CPyObject(PyObject* p) : p(p)
	{}

	CPyObject(CPyObject& other) : p(other.AddRef())
	{ }

	~CPyObject()
	{
		Release();
	}

	PyObject* getObject()
	{
		return p;
	}

	PyObject* setObject(PyObject* _p)
	{
		return (p = _p);
	}

	PyObject* AddRef()
	{
		if (p)
			Py_INCREF(p);
		
		return p;
	}

	void Release()
	{
		if (p)
			Py_DECREF(p);

		p = nullptr;
	}

	PyObject* operator->()
	{
		return p;
	}

	operator PyObject*()
	{
		return p;
	}

	PyObject* operator = (PyObject* pp)
	{
		p = pp;
		return p;
	}
};
