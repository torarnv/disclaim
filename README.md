# disclaim

[![Continuous Integration][ci-badge]][ci-link]
[![License][license-badge]][bsd]

`disclaim` is a macOS command-line tool that enables launching processes
with their own responsibility for Transparency, Consent, and Control (TCC)
permissions, _disclaiming_ any responsibility for the launched process from
the parent process.

## Usage

```shell
❯ disclaim <executable> [arguments...]
```

## Background

Mac OS X Mountain Lion (10.8) introduced a new Privacy tab to the Security &
Privacy pane of System Preferences, giving the user granular access control
to features such as contacts or location on an application by application
basis. Since then the list of permissions has grown significantly, and is
today a key component of the application sandboxing on macOS.

The permissions are managed by the `tccd` service, which is part of the
Transparency, Consent, and Control (TCC) subsystem. When an application
requests a feature that is controlled by TCC, the system checks a central
database for whether the application has already been granted access, and
if not shows the user a dialog, asking them to grant or deny access.

An important aspect in this process is how TCC determines which application
is asking for the permission. Under normal circumstances, when an application
is launched via Finder, the process becomes a child of `launchd`, and is
the "responsible process", i.e. responsible for its own permissions.

But when an application launches its own child processes, those will defer
to its parent process for any permission responsibility. In the case of an
application embedding and launching helper executables this behavior makes
a lot of sense, but it can be a bit surprising when launching executables
from the terminal.

In that situation, the terminal application (Terminal, iTerm2, etc) is the
responsible process, so launching a child process such as `zsh`, or `python3`,
that in turn tries to access a feature guarded by TCC, will attribute the
access to the terminal application instead of the child executable.

This is where `disclaim` comes in. By launching the child executable in
a way that makes it responsible for its own permissions (making it its
own responsible process), the permissions can be correctly attributed
to the child process.

For more information, see [The Curious Case of the Responsible Process][blog].

## License

This software is licensed under the [BSD two-clause "simplified" license][bsd].

[blog]: https://www.qt.io/blog/the-curious-case-of-the-responsible-process "The Curious Case of the Responsible Process"

[bsd]: http://opensource.org/licenses/BSD-2-Clause "BSD two-clause license"
[license-badge]: https://img.shields.io/github/license/torarnv/disclaim?color=informational&label=License

[ci-badge]: https://github.com/torarnv/disclaim/actions/workflows/ci.yml/badge.svg
[ci-link]: https://github.com/torarnv/disclaim/actions/workflows/ci.yml
