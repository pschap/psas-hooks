#include <stdio.h>
#include <taihen.h>

#ifndef HOOKS_H
#define HOOKS_H

#define DEBUG 1
#define HASH_TABLE_SIZE 255

// Hooking Data
typedef struct Hook
{
    unsigned int offset;
    int (*detour)();
    tai_hook_ref_t ref;
    SceUID uid;
} Hook;

typedef struct HookNode
{
    Hook *hook;
    struct HookNode *next;
} HookNode;

// Detours
int detour_ReadFile(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_MountPsarc(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_IncrementAp(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_HashBcsvAttribute(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_LoadingThreadEntry(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_LoadAsset(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_FUN_81031ce0(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);

// Memory Dumps
void hex_dump(const char *desc, const void *addr, const int len, int perLine);

// Initialize table of hooking nodes
void init_hook_table();

// Insert hooks
void insert_hooks();

// Free table of hooking nodes
void free_hook_table();

// Release hooks
void release_hooks();

// Get the hook at the specified offset
Hook *resolve_hook_at_offset(unsigned int offset);

#endif