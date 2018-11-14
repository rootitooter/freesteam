# Interfaces
The Steamworks API defines several interfaces, all accessible through their
global accessors. For example, we can access the `ISteamClient` interface
through the `SteamClient()` accessor function.

These accessors are all defined in `steam_api_internal.h` in the form of inline
functions.

The function `SteamInternal_ModuleContext()` returns a `CSteamAPIContext` which
contains all the accessor functions. So, if we were to get the `SteamClient`
accessor function, we'd do it by calling `SteamInternal_ModuleContext().SteamClient()`,
which returns a pointer to `ISteamClient`.

## `ISteamClient`
This interface deals with all communications with the server, and also includes
an especially useful method, `GetISteamGenericInterface`. This is used to get
a pointer to an `ISteamAppTicket`, which has the `GetAppOwnershipData` function,
which is used by the Steam DRM to determine ownership.

## `SteamInternal_ModuleContext`
This function seems to be the answer to overriding interface methods, which will
be required for cracking DRM.

Let's look at its definition:

```cpp
inline CSteamAPIContext& SteamInternal_ModuleContext()
{
	static void* s_CallbackCounterAndContext[ 2 + sizeof(CSteamAPIContext)/sizeof(void*) ] = { (void*)&SteamInternal_OnContextInit, 0 };
	return *(CSteamAPIContext*)SteamInternal_ContextInit( s_CallbackCounterAndContext );
}
```

We can see that it calls `SteamInternal_ContextInit`, which is unfortunately not
documented, but just from guessing, it seems like it's just a wrapper around
`CSteamAPIContext::Init`, yet another inline function. Here, it calls
`SteamInternal_CreateInterface`, which returns `ISteamClient*` and is exported in
the shared library, which means we can override it!

This function seems like it will be the answer to our problem.