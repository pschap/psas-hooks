#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <taihen.h>
#include <psp2/kernel/modulemgr.h>

#include "hooks.h"

/*
 * Add an entry in this table for each offset in the game's 
 * code for which you wish to insert a hook. 
 *
 * The first entry should be the offset minus the base
 * address of the game (for PSABR, this is 0x81000000).
 *
 * The second entry should be the "detour" function that 
 * should be executed when the hook is triggered.
 *
 * Initialize ref (3) and uid (4) struct members as NULL for now,
 * they will be reinitialized in psas_hooks.c when hooks
 * are inserted.
 */ 
static Hook hooks[] = 
{
//    {0x86b38, detour_ReadFile, 0, 0},
//    {0x8748a, detour_MountPsarc, 0, 0},
//    {0xcfa86, detour_IncrementAp, 0, 0},
//    {0x317a8, detour_HashBcsvAttribute, 0, 0},
//    {0x31ce0, detour_FUN_81031ce0, 0, 0},
//    {0x86622, detour_LoadingThreadEntry, 0, 0},
//    {0x85f46, detour_LoadAsset, 0, 0}
};

static HookNode **table = NULL;

void hex_dump(const char *desc, const void *addr, const int len, int perLine)
{
    unsigned int i;
    unsigned int idx;
    unsigned char buff[perLine + 1];
    const unsigned char *pc = (const unsigned char *)addr;
    const unsigned int base = (const unsigned int)addr;

    // Silently ignore silly per-line values
    if (perLine < 4 || perLine > 64)
        perLine = 16;

    // Output description if given
    if (desc)
        printf("%s:\n", desc);

    // Length checks
    if (len == 0)
    {
        printf("\tZERO LENGTH\n");
        return;
    }

    if (len < 0)
    {
        printf("\tNEGATIVE LENGTH\n");
        return;
    }

    // Process every byte in data
    for (i = 0, idx = 0; i < len; i++)
    {
        if (i == 0)
            printf("  %04x: ", base + i);

        // Multiple of perLine means new or first line (with line offset)
        if (idx == perLine)
        {
            idx = 0;
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset of current line
            printf("  %04x: ", base + i);
        }

        // Now the hex code for the specific character
        printf(" %02x", pc[i]);

        // And buffer a printable ASCII character for later
        if (pc[i] < 0x20 || pc[i] > 0x7e)
            buff[idx] = '.';
        else
            buff[idx] = pc[i];

        buff[idx + 1] = '\0';
        idx++;
    }

    // Pad out the last line if not exact perLine characters
    while (idx != perLine)
    {
        printf ("   ");
        idx++;
    }

    // And print the final ASCII buffer
    printf("  %s\n", buff);
}

void init_hook_table()
{
    Hook *hook;
    HookNode *entry;
    HookNode *new_entry;
    unsigned int i, idx, num_hooks;

    // Initialize table
    table = (HookNode **)malloc(HASH_TABLE_SIZE * sizeof(HookNode *));
    if (!table)
        return;

    memset(table, 0, HASH_TABLE_SIZE * sizeof(HookNode *));

    // Insert hooks into table
    num_hooks = sizeof(hooks) / sizeof(Hook);
    for (i = 0; i < num_hooks; i++)
    {
        hook = &hooks[i];
        idx = hook->offset & HASH_TABLE_SIZE;
        entry = table[idx];

        new_entry = (HookNode *)malloc(sizeof(HookNode));
        new_entry->hook = hook;
        new_entry->next = NULL;

        if (!entry)
            table[idx] = new_entry;
        else
        {
            while (entry->next)
                entry = entry->next;

            entry->next = new_entry;
        }
    }
}

void insert_hooks(tai_module_info_t *tai_info)
{
    Hook *hook;
    unsigned int i;
    unsigned int num_hooks;

    num_hooks = sizeof(hooks) / sizeof(Hook);
    for (i = 0; i < num_hooks; i++)
    {
        hook = &hooks[i];
        hook->uid = taiHookFunctionOffset(&hook->ref, tai_info->modid, 0, hook->offset, 1, hook->detour);
        if (DEBUG)
            printf("hook_uid[%i]: %x @ 0x%x\n", i, hook->uid, 0x81000000 + hook->offset);
    }
}

void free_hook_table()
{
    HookNode *entry;
    HookNode *tmp;
    unsigned int i;

    for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
        entry = table[i];
        while (entry)
        {
            tmp = entry;
            entry = entry->next;
            free(tmp);
            tmp = NULL;
        }
    }

    free(table);
    table = NULL;
}

Hook *resolve_hook_at_offset(unsigned int offset)
{
    HookNode *entry;
    unsigned int idx;

    idx = offset & HASH_TABLE_SIZE;
    entry = table[idx];
    while(entry->hook->offset != offset)
        entry = entry->next;

    return entry ? entry->hook : NULL;
}

void release_hooks()
{
    Hook *hook;
    unsigned int i;
    unsigned int num_hooks;

    num_hooks = sizeof(hooks) / sizeof(Hook);
    for (i = 0; i < num_hooks; i++)
    {
        hook = &hooks[i];
        if (hook->uid >= 0)
            taiHookRelease(hook->uid, hook->ref);
    }
}
