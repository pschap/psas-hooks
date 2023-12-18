#include <stdio.h>
#include <taihen.h>
#include <psp2/kernel/modulemgr.h>

#include "hooks.h"

#define DEBUG 1

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
    unsigned int i;
    Hook *hook;
    tai_module_info_t tai_info;

    tai_info.size = sizeof(tai_module_info_t);
    taiGetModuleInfo(TAI_MAIN_MODULE, &tai_info);

    // Insert hooks
    for (i = 0; i < NUM_HOOKS; i++)
    {
        hook = &hooks[i];
        hook->uid = taiHookFunctionOffset(&hook->ref, tai_info.modid, 0, hook->offset, 1, hook->detour);
        if (DEBUG)
            printf("hook_uid[%i]: %x\n", i, hook->uid);
    }

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
    unsigned int i;
    Hook *hook;

    // Release hooks
    for (i = 0; i < NUM_HOOKS; i++)
    {
        hook = &hooks[i];
        if (hook->uid >= 0)
            taiHookRelease(hook->uid, hook->ref);
    }

    return SCE_KERNEL_STOP_SUCCESS;
}
