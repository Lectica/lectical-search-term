#include <Python.h>
#include <regex.h>


int _check_list_of_numbers(PyObject *lst, void *address) {
  PyObject *item = NULL;

  if (!lst || !PyList_Check(lst)) { /* Note: PyList_Check allows sub-types. */
    return 0;
  }
  for (Py_ssize_t i = 0; i < PyList_GET_SIZE(lst); ++i) {
    item = PyList_GetItem(lst, i);
    if (!(PyLong_CheckExact(item) || PyFloat_CheckExact(item) ||
          PyComplex_CheckExact(item))) {
      PyErr_Format(PyExc_ValueError, "Item %d is not a number.", i);
      return 0;
    }
  }
  return 1; /* Success. */
}

void print_result(int return_value){
  if (return_value == 0){
    printf("Pattern found.\n");
  }
  else if (return_value == REG_NOMATCH){
    printf("Pattern not found.\n");
  }
  else{
    printf("An error occured.\n");
  }
}


static PyObject* helloworld(PyObject* self, PyObject * args) {
    // receive 2 parameters. List of terms and text
    char regex_str[256];
    const char * text = NULL;    PyObject * ret = NULL;    PyObject * pyListOfItems = NULL; PyObject * string;
    PyObject *listOfTerms = PyList_New(0);
    const char * item;
    // for regex
    regex_t regex;
    int regex_return;

    // http://web.mit.edu/people/amliu/vrut/python/ext/parseTuple.html
    if (!PyArg_ParseTuple(args, "sO", &text, &pyListOfItems)) {
        goto except;
    }

    for (Py_ssize_t i = 0; i < PyList_GET_SIZE(pyListOfItems); i++) {
        string = PyList_GetItem(pyListOfItems, i);
        if (string == NULL) {printf("Algum erro que eu nao sei o que eh\n"); }
        ret = PyUnicode_AsUTF8String(string);
        item = PyBytes_AsString(ret);

        sprintf(&regex_str, "[[:<:]]%s[[:>:]]", item);
        regex_return = regcomp(&regex, regex_str, REG_EXTENDED);
        regex_return = regexec(&regex, text, 0, NULL, 0);

        if (0 == regex_return) {
            PyList_Append(listOfTerms, string);
        }

        /* Free memory allocated to the pattern buffer by regcomp() */
        regfree(&regex);
    }

    return listOfTerms;

    // return list of items found
    except:
        Py_XDECREF(ret);
        ret = NULL;
}

// PyDOC_STRVAR(lec_doc, "Search list of terms inside text. Return terms found in text.");

static PyMethodDef myMethods[] = {
    {
        "helloworld",  /* The function name in python */
        helloworld,    /* The C function to invoke. */
        METH_VARARGS,  /* Flag telling Python how to invoke ``helloworld`` */
        "Search list of items inside text. Return items found in text."   /* The docstring as a C string. */
    },
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef myModule = {
    PyModuleDef_HEAD_INIT,
    "MyModule",
    "Test module",
    -1, // global state
    myMethods
};

PyMODINIT_FUNC PyInit_myModule(void){
    return PyModule_Create(&myModule);
}
