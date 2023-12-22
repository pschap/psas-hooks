#include <stdio.h>
#include <vitasdk.h>
#include <taihen.h>

#include "hooks.h"

int detour_ReadFile(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0x86b38);
    printf("[hook] ReadFile     char *file: %x, int *fileSize: %x\n", r0, r1);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));
    if (r0)
        printf("\tReading file: %s\n", (char *)r0);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_MountPsarc(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0x8748a);
    printf("[hook] MountPsarc       r0: %x, char *path: %x\n", r0, r1);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));
    if (r1)
        printf("\tMounting PSARC: %s\n", (char *)r1);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_FUN_810d8da4(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0xd8da4);
    printf("[hook] FUN_810d8da4      r0: %x, r1: %x, r2: %x\n", r0, r1, r2);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));
    hex_dump("param_2", (void *)r1, 0x100, 16);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_FUN_810cfa86(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0xcfa86);
    printf("[hook] FUN_810cfa86   r0: %x, r1: %x\n", r0, r1);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));
    
    // This will prevent AP bursts from occurring
    // *(unsigned int *)(r1 + 0x5c) = 0x819740c0;

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}
