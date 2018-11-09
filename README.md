# freesteam
freesteam allows you to run Steam games outside of their intended environment
(e.g. without steam running, or on a computer where Steam isn't installed).

It is currently intended for Linux only, and has a long way to go before it's
stable.

## How to use
Compile `shim.so` and then use it as LD_PRELOAD for any Steam game.

```bash
make
LD_PRELOAD=./out/shim.so $GAMEPATH
```

## How it works
This works by utilizing Linux's LD_PRELOAD environment variable, which lets you
override a shared libraries' functions without having its source code by
redefining them in a separate shared library.

## License (LGPLv3)
Anyone is free to contribute to this project.

See LICENSE for full license.