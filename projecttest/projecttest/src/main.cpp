#include <Python.h>
#include <iostream>

#include "pyhelper.hpp"

int main()
{
	//from a string
	{
		Py_Initialize();
		PyRun_SimpleString("print(\"hello from string\")");
		Py_Finalize();
	}
	//from a file
	{
		char filename[] = "myscript.py";
		Py_Initialize();
		PyObject* obj = Py_BuildValue("s", filename);
		FILE* file = _Py_fopen_obj(obj, "r+");
		if (file != NULL) { PyRun_SimpleFile(file, filename);
		}Py_Finalize();
	}
	//call python function from c++
	{
		CPyInstance instance;
		CPyObject pName = PyUnicode_FromString("myfunc");
		CPyObject pModule = PyImport_Import(pName);
		if (pModule)
		{
			CPyObject pFunc = PyObject_GetAttrString(pModule, "getInteger");
			if (pFunc && PyCallable_Check(pFunc))
			{
				CPyObject pValue = PyObject_CallObject(pFunc, NULL);//NULL == pas d arguments
				printf_s("C: getfloat() = %d\n", _PyLong_AsInt(pValue));
			}
		}
	}
	//call c++ function from python
	{
		char filename[] = "myimportscript.py";
		Py_Initialize();
		PyObject* obj = Py_BuildValue("s", filename);
		FILE* file = _Py_fopen_obj(obj, "r+");
		if (file != NULL) {
			PyRun_SimpleFile(file, filename);
		}Py_Finalize();
	}

	char c;
	std::cin >> c;
	return 0;
}