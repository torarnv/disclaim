// Copyright (C) 2024 Tor Arne Vestbø
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

extern "C" int responsibility_spawnattrs_setdisclaim(posix_spawnattr_t attrs, int disclaim)
    __attribute__((availability(macos,introduced=10.14),weak_import));

#define CHECK_SPAWN(expr) \
    if (int err = (expr)) { \
        posix_spawnattr_destroy(&attr); \
        fprintf(stderr, "disclaim failed to launch %s: %s", executable, strerror(err)); \
        exit(err); \
    }

int main(int argc, char *const *argv, char *const *envp)
{
    if (argc < 2) {
        printf("usage: disclaim <executable> [arguments...]\n");
        return 1;
    }

    const char *executable = argv[1];
    ++argv;

    posix_spawnattr_t attr;
    CHECK_SPAWN(posix_spawnattr_init(&attr));

    // Behave as exec
    short flags = POSIX_SPAWN_SETEXEC;

    // Reset signal mask
    sigset_t no_signals;
    sigemptyset(&no_signals);
    CHECK_SPAWN(posix_spawnattr_setsigmask(&attr, &no_signals));
    flags |= POSIX_SPAWN_SETSIGMASK;

    // Reset all signals to their default handlers
    sigset_t all_signals;
    sigfillset(&all_signals);
    CHECK_SPAWN(posix_spawnattr_setsigdefault(&attr, &all_signals));
    flags |= POSIX_SPAWN_SETSIGDEF;

    CHECK_SPAWN(posix_spawnattr_setflags(&attr, flags));

    if (__builtin_available(macOS 10.14, *)) {
        // Disclaim TCC responsibilities for parent, making
        // the launched process the responsible process.
        if (responsibility_spawnattrs_setdisclaim)
            CHECK_SPAWN(responsibility_spawnattrs_setdisclaim(&attr, 1));
    }

    CHECK_SPAWN(posix_spawnp(nullptr, executable, nullptr, &attr, argv, envp));
}
