#include <stdio.h>
#include <vitasdk.h>
#include <taihen.h>

#include "hooks.h"

int detour_ReadFile(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0x86b38);
    printf("[hook] ReadFile     char *file: %x, int *fileSize: %x\n", r0, r1);

    if (r0)
        printf("\tReading file: %s\n", (char *)r0);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_MountPsarc(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0x8748a);
    printf("[hook] MountPsarc       r0: %x, char *path: %x\n", r0, r1);

    if (r1)
        printf("\tMounting PSARC: %s\n", (char *)r1);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_IncrementAp(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    float s0;
    Hook *hook;

    // Use inline assembly to get the value of floating-point register s0
    asm volatile("vmov.f32 %0, s0" : "=r" (s0));

    hook = resolve_hook_at_offset(0xcfa86);
    printf("[hook] IncrementAp  float ap: %a, r0: %x\n", s0, r0);

    // This will prevent AP bursts from occurring
    // *(unsigned int *)(r0 + 0x5c) = 0x819740c0;

    printf("\tAP Amount: %f\n", s0);
    printf("\tReturn Address: %p\n", __builtin_return_address(0));
    hex_dump("param_2 + 0x6f8", (void *)(r1 + 0x6f8), 0x8, 16);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_HashBcsvAttribute(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0x317a8);
    printf("[hook] HashBcsvAttribute    char *attribute_name: %x, uint key: %x\n", r0, r1);

    if (r0)
        printf("\tAttribute Name: %s\n", (char *)r0);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_FUN_81031ce0(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;

    hook = resolve_hook_at_offset(0x31ce0);
    printf("[hook] FUN_81031ce0     r0: %x\n", r0);

    printf("\t Return Address: %p\n", __builtin_return_address(0));
    if (r0)
        hex_dump("param_1", (void *)r0, 64, 16);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_LoadingThreadEntry(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;
    void *load_assets;

    hook = resolve_hook_at_offset(0x86622);
    printf("[hook] LoadingThreadEntry   r0: %x\n", r0);

    printf("\t Return Address: %p\n", __builtin_return_address(0));
    if (r0)
        hex_dump("param_1", (void *)r0, 1024, 16);
    
    load_assets = *(void **)(r0 + 0x10);
    if (load_assets)
        hex_dump("load_assets", load_assets, 1024, 16);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}

int detour_LoadAsset(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3)
{
    Hook *hook;
    
    hook = resolve_hook_at_offset(0x85f46);
    printf("[hook] LoadAsset     r0: %x, r1: %x, r2: %x, r3: %x\n", r0, r1, r2, r3);

    printf("\tReturn Address: %p\n", __builtin_return_address(0));

    printf("\tLoading File: %s\n", (char *)r0);
    printf("\tLoading Asset: %s\n", (char *)r1);

    //if (r2)
        //hex_dump("param_3", (void *)r2, 32, 16);

    //if (r3)
        //hex_dump("param_4", (void *)r3, 32, 16);

    return TAI_CONTINUE(int, hook->ref, r0, r1, r2, r3);
}
