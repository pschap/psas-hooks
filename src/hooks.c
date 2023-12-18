#include <taihen.h>

#include "hooks.h"

Hook hooks[NUM_HOOKS] = 
{
    {0x86b38, detour_ReadFile, 0, 0},
    {0x8748a, detour_MountPsarc, 0, 0}
};

tai_hook_ref_t resolve_hook_ref(unsigned int offset)
{
    unsigned int i;
    Hook *tmp;
    tai_hook_ref_t ref = 0;

    for (i = 0; i < NUM_HOOKS; i++)
    {
        tmp = &hooks[i];
        if (tmp->offset == offset)
        {
            ref = tmp->ref;
            break;
        }
    }

    return ref;
}