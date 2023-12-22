# PSAS Hooks

PSAS Hooks is a tool that allows for easily inserting function hooks in the game PlayStation All-Stars Battle Royale for the Sony PlayStation Vita. 

## Demo
[![Function Hooking Demo](https://img.youtube.com/vi/75TMrhtZ7E4/0.jpg)](https://www.youtube.com/watch?v=75TMrhtZ7E4)

## What is Function Hooking?
"The term 'hooking a function' is the process of changing the default flow of execution,
usually with the intent of either gathering information or changing the result of the hooked function entirely."

Using this tool, hooks can be inserted into individual functions within the game that can be used print additional debugging information, dump memory, or alter the behavior or memory state of the game allowing for custom mods. This tool takes advantage of the taihen function hooking framework to insert hooks at specific addresses within the game's code.

## Usage
To be able to capture and view debugging messages printed to stdout, first install PrincessLog from the [PS Vita RE-Tools Repository](https://github.com/TeamFAPS/PSVita-RE-tools).
There are three files:

1. ```NetDbgLogPc.exe```
2. ```NetLoggingMgrSettings.vpk```
3. ```net_logging_mgr.skprx```.

Transfer ```NetLoggingMgrSettings.vpk to your Vita and install it.``` This will allow you to configure PrincessLog to redirect stdout to a specific IP and port. The implementation of PrincessLog in this repository seems to have a horrible bug in it where a null byte is not read into the configuration file header, resulting in the configuration file not being loaded/PrincessLog failing to start. If you experience difficulty with PrincessLog, you unfortunately will probably have to open the configuration file in hex editor and manually write this null byte. The configuration file should be located on your Vita in ```ur0:data/NetLoggingMgrConfig.bin```. The first 4 bytes of the configuration file should be the following:

```
00000000: 4e4c 4d00      NLM.
```

Next, add ```net_logging_mgr.skprx``` to your ```ur0:tai/config.txt```.

```
*KERNEL
ur0:tai/net_logging_mgr.skprx
```

Lastly, if on Windows, run ```NetDbgLogPc.exe``` from the command prompt. The port you specified earlier in the PrincessLog configuration file can be specified here as a command-line argument. If on Linux, use the netcat command ```nc -kl <port>```. Add the compiled hooking module (```psas-hooks.suprx```) to your taihen configuration file.
You must add a new section to this file with the ID for PlayStation All-Stars. For example, for the American version of the game, add the following:

```
*PCSA00069
ur0:tai/psas-hooks.suprx
```

If done correctly, you should see all stdout output from your Vita being printed to your terminal as in the demo video above.

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

In ```hooks.h```, add a definition for the "detour" to execute when the hook is triggered/instructions are executed at a given offset. Each detour function must return an int and the arguments passed to this function will be the contents of ARM registers r0-r3. For hooks that occur at the beginning of a function, r0-r3 will hold the function arguments.

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

Entries in this table can simply be commented out to disable individual hooks.

In ```detours.c```, provide definitions for the detour functions previously defined in ```hooks.h```:

```c
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
```
