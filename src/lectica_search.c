
#include <Python.h>
#include <regex.h>
#include <string.h>

#define REGEX_STR_LEN 128


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

int search(const char * text, const char * item) {
    char regex_str[REGEX_STR_LEN];
    int regex_return;
    regex_t regex;

    memset(regex_str, 0, sizeof(char) * REGEX_STR_LEN);
    // sprintf(regex_str, "[[:<:]]%s[[:>:]]", item);
    // sprintf(regex_str, "\\b%s\\b", item);
    //sprintf(regex_str, " [.-]*%s[,.?!;-]* ", item);
    sprintf(regex_str, " [a-z.,-]*%s[a-z,.?:!;-]* ", item);

    regex_return = regcomp(&regex, regex_str, REG_EXTENDED | REG_NOSUB);
    if (0 != regex_return) {
        char buffer[100];
        printf ("Regex error compiling [%s] \n", regex_str);
        regerror(regex_return, &regex, buffer, 100);
        printf("regcomp() failed with '%s'\n", buffer);
        /* cannot return same value from regcomp() */
        return -255;
    }
    regex_return = regexec(&regex, text, 0, NULL, 0);

    /* Free memory allocated to the pattern buffer by regcomp() */
    regfree(&regex);

    return regex_return;
}


static PyObject* search_terms_in_text(PyObject* self, PyObject * args) {
    char regex_str[REGEX_STR_LEN];
    // receive 2 parameters. List of terms and text
    const char * text = NULL;
    const char * item;
    int regex_return = 0;
    PyObject * ret = NULL;
    PyObject * pyListOfItems = NULL;
    PyObject * string;
    PyObject *listOfTerms = PyList_New(0);

    // http://web.mit.edu/people/amliu/vrut/python/ext/parseTuple.html
    if (!PyArg_ParseTuple(args, "sO", &text, &pyListOfItems)) {
        goto except;
    }

    // Make sure we own the GIL
    // Use functions in the interpreter
    PyGILState_STATE state = PyGILState_Ensure();

    for (Py_ssize_t i = 0; i < PyList_GET_SIZE(pyListOfItems); i++) {
        string = PyList_GetItem(pyListOfItems, i);
        if (string == NULL) {printf("Algum erro que eu nao sei o que eh: index [%zd] list len [%zd]\n", i, PyList_GET_SIZE(pyListOfItems)); }
        ret = PyUnicode_AsUTF8String(string);
        item = PyBytes_AsString(ret);

        memset(regex_str, NULL, sizeof(char) * REGEX_STR_LEN);
        sprintf(regex_str, " %s ", item);

        if (strstr(text, regex_str)) {
            PyList_Append(listOfTerms, string);
        }
    }

    // Restore previous GIL state and return
    PyGILState_Release(state);

    return listOfTerms;

    // return list of items found
    except:
        Py_XDECREF(ret);
        ret = NULL;
        Py_XDECREF(listOfTerms);
}

static PyMethodDef myMethods[] = {
    {
        "search_terms_in_text",  /* The function name in python */
        search_terms_in_text,    /* The C function to invoke. */
        METH_VARARGS,            /* Flag telling Python how to invoke ``search_terms_in_text`` */
        "Search list of terms inside text. Text has to be in lower case. Return terms found in text."   /* The docstring as a C string. */
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef lectica = {
    PyModuleDef_HEAD_INIT,
    "lectica",
    "Lectica module",
    -1, // global state
    myMethods
};

PyMODINIT_FUNC PyInit_lectica(void){
    return PyModule_Create(&lectica);
}
