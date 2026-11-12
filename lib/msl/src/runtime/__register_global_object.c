// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/cpp/src/__register_global_object.c:
// ported as it is, with credit (a project decision).
#include "MWException.h"

#define CALL_DTOR(dtor, obj) (((void (*)(void *, int))(dtor))((obj), -1))

DestructorChain *__global_destructor_chain;

void *__register_global_object(void *object, void *destructor, DestructorChain *link) {
    link->next   = __global_destructor_chain;
    link->dtor   = destructor;
    link->object = object;

    __global_destructor_chain = link;
    return object;
}
