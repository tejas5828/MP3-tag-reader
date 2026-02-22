#ifndef TYPE_H
#define TYPE_H

typedef enum
{
    v_failure,
    v_success,
    e_success,
    e_failure
}Song;

typedef enum
{
   s_view,
   s_edit,
   s_help,
   s_unsupported
}Operationtype;
#endif