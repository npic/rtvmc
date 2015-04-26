#ifndef ERROR_H
#define ERROR_H

void log_error(const char * message);

#define FAIL(errcode, errmsg) \
    do                        \
    {                         \
        log_error((errmsg));  \
        return (errcode);     \
    } while (0)

#define CHECK_NOT_NULL(val, errcode, errmsg) \
    do                                       \
    {                                        \
        if((val) == NULL)                    \
        {                                    \
            FAIL((errcode), (errmsg));       \
        }                                    \
    } while (0)

#define CHECK_OK(val, errmsg)                       \
    do                                              \
    {                                               \
        if(((val) != OK) && ((val) != OK_NEGATIVE)) \
        {                                           \
            FAIL((val), (errmsg));                  \
        }                                           \
    } while(0)

#define CHECK_OK_POSITIVE(val, errmsg) \
    do                                 \
    {                                  \
        if((val) != OK)                \
        {                              \
            FAIL((val), (errmsg));     \
        }                              \
    } while (0)

#define CHECK_NOT_FERROR(val)        \
    do                               \
    {                                \
        if(ferror(val))              \
        {                            \
            FAIL(IO, "I/O error\n"); \
        }                            \
    } while (0)

enum errcode_t
{
    OK,
    OK_NEGATIVE,
    IO,
    MEM_OVERFLOW,
    BAD_OPCODE,
    BAD_ARGUMENT
};
typedef enum errcode_t errcode_t;

#endif

