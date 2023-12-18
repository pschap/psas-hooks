#include <stdio.h>
#include <vitasdk.h>
#include <taihen.h>

#include "hooks.h"

int detour_ReadFile(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    int ret;
    tai_hook_ref_t ref;

    printf("[hook] ReadFile     char *file: %x, int *fileSize: %x\n", r0, r1);

    if (r0)
        printf("\tReading file: %s\n", (char *)r0);

    ref = resolve_hook_ref(0x86b38);
    ret = TAI_CONTINUE(int, ref, r0, r1, r2, r3);

    return ret;
}

int detour_MountPsarc(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    int ret;
    tai_hook_ref_t ref;

    printf("[hook] MountPsarc       r0: %x, char *path: %x\n", r0, r1);

    if (r1)
        printf("\tMounting PSARC: %s\n", (char *)r1);

    ref = resolve_hook_ref(0x8748a);
    ret = TAI_CONTINUE(int, ref, r0, r1, r2, r3);

    return ret;
}
