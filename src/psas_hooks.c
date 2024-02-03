#include <stdio.h>
#include <taihen.h>
#include <psp2/kernel/modulemgr.h>

#include "hooks.h"

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
    tai_module_info_t tai_info;

    tai_info.size = sizeof(tai_module_info_t);
    taiGetModuleInfo(TAI_MAIN_MODULE, &tai_info);

    // Set-up hooks
    insert_hooks(&tai_info);
    init_hook_table();

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
    // Release hooks
    release_hooks();
    free_hook_table();

    return SCE_KERNEL_STOP_SUCCESS;
}
