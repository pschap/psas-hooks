#include <taihen.h>

#ifndef HOOKS_H
#define HOOKS_H

#define NUM_HOOKS 2

// Hooking Data
typedef struct Hook
{
    unsigned int offset;
    int (*detour)();
    tai_hook_ref_t ref;
    SceUID uid;
} Hook;

// Detours
int detour_ReadFile(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_MountPsarc(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);

// Hooks
extern Hook hooks[NUM_HOOKS];

// Find hook ref at the specified offset
tai_hook_ref_t resolve_hook_ref(unsigned int offset);

#endif