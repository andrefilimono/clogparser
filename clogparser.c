#include <Python.h>
#include <ctype.h>

typedef enum delitimer {
    SPACES,
    BRACES,
    QUOTES
} delimiter_t;

typedef enum icecastToken {
    IP,
    DASH,
    AUTH,
    DATE,
    REQUEST,
    STATUS,
    BYTES_SENT,
    REFERER,
    USER_AGENT,
    DURATION,
    NUM_ICECAST_TOKENS
} icecastToken_t;

static delimiter_t
getIcecastDelimiterByToken(icecastToken_t token) {
    delimiter_t delimiter;
    switch (token) {
        case IP:
        case DASH:
        case AUTH:
        case STATUS:
        case BYTES_SENT:
        case DURATION:
            delimiter = SPACES;
            break;
        case DATE:
            delimiter = BRACES;
            break;
        case REQUEST:
        case REFERER:
        case USER_AGENT:
            delimiter = QUOTES;
            break;
        default:
            delimiter = SPACES;
    }
    return delimiter;
}

static PyObject *
parseIcecastLine(PyObject *self, PyObject *args) {
    const char *line;
    if (!PyArg_ParseTuple(args, "s", &line)) {
        return NULL;
    }

    PyObject *result = PyList_New(NUM_ICECAST_TOKENS);

    icecastToken_t currentToken = IP;
    Py_ssize_t tokenLength = 0;
    const char *innerIterator;
    const char *p = line;

    while (*p && currentToken < NUM_ICECAST_TOKENS) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        tokenLength = 0;
        innerIterator = p;
        switch (getIcecastDelimiterByToken(currentToken)) {
            case SPACES:
                while (*innerIterator && !isspace(*innerIterator)) {
                    innerIterator++;
                    tokenLength++;
                }
                break;
            case BRACES:
                if (*innerIterator == '[') {
                    innerIterator++;
                    p++;
                }

                while (*innerIterator && *innerIterator != ']') {
                    innerIterator++;
                    tokenLength++;
                }
                innerIterator++;
                break;
            case QUOTES:
                while(*innerIterator && *innerIterator == '"') {
                    innerIterator++;
                    p++;
                }

                while (*innerIterator) {
                    if (*innerIterator == '"') {
                        char nextChar = *(innerIterator + 2);
                        if (isdigit(nextChar) || nextChar == '"') {
                            if (*(innerIterator - 1) == '"') {
                                tokenLength--;
                            }
                            break;
                        }
                    }
                    innerIterator++;
                    tokenLength++;
                }
                innerIterator++;
                break;
        }

#if PY_MAJOR_VERSION >= 3
        PyObject *token = PyUnicode_FromStringAndSize(p, tokenLength);
#else
        PyObject *token = PyString_FromStringAndSize(p, tokenLength);
#endif
        PyList_SetItem(result, currentToken++, token);
        p = innerIterator;
    }

    if (currentToken < NUM_ICECAST_TOKENS) {
        Py_XDECREF(result);
        Py_RETURN_NONE;
    }

    return result;
}

static PyMethodDef methods[] = {
    {"parse_icecast",  parseIcecastLine, METH_VARARGS, "Parse a line of Icecast access log."},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "clogparser",
    NULL,
    -1,
    methods
};
#endif

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
PyInit_clogparser(void)
#else
initclogparser(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    (void) Py_InitModule("clogparser", methods);
#endif

#if PY_MAJOR_VERSION >= 3
    if (module == NULL) {
        return NULL;
    }
    return module;
#endif
}
