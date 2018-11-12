# SteamAPI_Init

This file (attempts to) describe what goes on in this function as per the latest
version of the Steamworks API.

## Official description
```cpp
S_API bool S_CALLTYPE SteamAPI_Init();
```

Initializes the Steamworks API.

**Returns**: bool

**true** indicates that all required interfaces have been acquired and are
accessible.

**false** indicates one of the following conditions:
- The Steam client isn't running. A running Steam client is required to
provide implementations of the various Steamworks interfaces.
- The Steam client couldn't determine the App ID of game. If you're running your
application from the executable or debugger directly then you must have a
`steam_appid.txt` in your game directory next to the executable, with your app
ID in it and nothing else. Steam will look for this file in the current working
directory. If you are running your executable from a different directory you may
need to relocate the steam_appid.txt file.
- Your application is not running under the same OS user context as the Steam
client, such as a different user or administration access level.
- Ensure that you own a license for the App ID on the currently active Steam
account. Your game must show up in your Steam library.
- Your App ID is not completely set up, i.e. in `Release State: Unavailable`, or
it's missing default packages.

## Reverse engineering
```asm
; var int local_18h @ rsp+0x18
push rbp ; start prelogue
push rbx
sub rsp, 0x28 ; end prelogue
mov rax, qword fs:[0x28]
mov qword [local_18h], rax
xor eax, eax
cmp qword [0x0024b380], 0
je 0x1cdf0
```
This function begins by checking if the value at address `0x24b380` is 0. I
presume this is a function that checks whether the steam API has been
initialized already, as this address is set to a new value in the block the last
jump instruction jumps to.

```asm
0x1cdf0:
lea rdi, [0x0024b350] ; rdi = 0x0024b350, argument #1 for sub.memset_a20
xor edx, edx          ; edx = 0, argument #2 for sub.memset_a20
xor esi, esi          ; esi = 0, argument #3 for sub.memset_a20
lea rcx, str.SteamClient017 ; 0x355f9 ; "SteamClient017" ; arg4 for sub.memset_a20
call sub.connect_to_client ; args: (0x24b350, 0, 0, str.SteamClient017)
mov rdi, rax
mov qword [0x0024b380], rax ; [0x24b380:8]=0
xor eax, eax
test rdi, rdi ; rdi == 0?
je 0x1cdd5
```

Here, we call the function that I called `connect_to_client`, as it seems to be
the one responsible for connecting to Steam's client. This is probably what we
are looking for if we're trying to bypass it.

### connect_to_client
I have yet to analyze this function in great detail, however, I can tell just
from glancing over at it that it calls the following functions:
 - `SteamAPI_GetSteamInstallPath` (implemented!)
 - `SteamAPI_IsSteamRunning` (implemented!)
 - `Steam_ReleaseThreadLocalMemory` (through dlsym)
 - `CreateInterface` (through dlsym)

The last two functions are loaded from some file file, which is found through a
separate subroutine. I'm assuming this file is `steamclient.so`, present in any
Steam client installation, as it is later referenced in the function through
dlclose, and also exports both functions mentioned.

I am unsure of how I'll handle these connections to the client, as there
obviously isn't one. I believe the best way to go about it would be to
reimplement this connect_to_client subroutine, but since it isn't exported
anywhere in the file, as far as I know it's next to impossible to override with
LD_PRELOAD. This leaves us with two options:
 a) Find a way to fake `steamclient.so`'s presence, and then intercept both
 function calls
 b) Reimplement SteamAPI_Init in its entirety

I'd much prefer to do a), but I am afraid it might not be the easiest. It'd
probably involve including a separate `steamclient.so` which would be copied
into the freesteam install dir. This, however, lets us intercept all sorts of
Steam client communications, which could for sure be beneficial.