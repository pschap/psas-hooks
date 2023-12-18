# PSAS Hooks

PSAS Hooks is a tool that allows for easily inserting function hooks in the game PlayStation All-Stars Battle Royale for the Sony PlayStation Vita. 

## Demo
[![Function Hooking Demo](https://img.youtube.com/vi/75TMrhtZ7E4/0.jpg)](https://www.youtube.com/watch?v=75TMrhtZ7E4)

## What is Function Hooking?
"The term 'hooking a function' is the process of changing the default flow of execution,
usually with the intent of either gathering information or changing the result of the hooked function entirely."

Using this tool, hooks can be inserted into individual functions within the game that can be used print additional debugging information, dump memory, or alter the behavior or memory state of the game allowing for custom mods. This tool takes advantage of the taihen function hooking framework to insert hooks at specific addresses within the game's code.

## Building
First install [CMake](https://cmake.org/download/) and then run the following commands from this repository.

1. ```mkdir build```
2. ```cd build```
3. ```cmake ..```
4. ```make all```

## Contributing/Adding Hooks
To add additional hooks, three files must be modified:

1. ```include/hooks.h```
2. ```src/detours.c```
3. ```src/hooks.c```

In ```hooks.h```, change the definition of ```NUM_HOOKS``` to reflect the total number of hooks present. Then, add definitions for the "detour" to execute when the hook is triggered/instructions are executed at a given offset. Each detour function must return an int and the arguments passed to this function will be the contents of ARM registers r0-r3. For hooks that occur at the beginning of a function, r0-r3 will hold the function arguments.

```c
#define NUM_HOOKS 2
...
// Detours
int detour_ReadFile(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);
int detour_MountPsarc(unsigned int r0, unsigned int r1, unsigned int r3, unsigned int r3);
```

In ```hooks.c```, an entry must be added to the ```hooks``` array that defines
the offset in the game's executable to insert the hook and the hook detour. For example, to add an entry for a function at address ```0x81086b38```
which reads from a file, add the following:

```c
Hook hooks[NUM_HOOKS] = 
{
    ...
    {0x86b38, detour_ReadFile, 0, 0},
    ...
};
```

In ```detours.c```, provide definitions for the detour functions previously defined in ```hooks.h```:

```c
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
```
