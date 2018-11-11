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