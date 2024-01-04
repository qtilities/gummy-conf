# Sqeleton

[![CI]](https://github.com/qtilities/sqeleton/actions/workflows/build.yml)

## Overview

Qt application template repository.

See the related [website page] for further information.

## Features

- Resource management following the Freedesktop [Desktop Entry] Specification
  file naming convention (icons, desktop and appdata files)
- [Appstream] metadata information
- Locale translations via [LXQt build tools]
- Install step

## Dependencies

Runtime:

- Qt5/6 base

Build:

- CMake
- Qt Linguist Tools
- Git (optional, to pull latest VCS checkouts)

## Build

`CMAKE_BUILD_TYPE` is usually set to `Release`, though `None` might be a [valid alternative][2].<br>
`CMAKE_INSTALL_PREFIX` has to be set to `/usr` on most operating systems.<br>
Using `sudo make install` is discouraged, instead use the system package manager where possible.

```bash
cmake -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr -W no-dev
cmake --build build --verbose
DESTDIR="$(pwd)/package" cmake --install build
```

## Licenses

- Sqeleton is licensed under the [MIT] license.
- Application icon is from [Openclipart], [CC0-1.0] license.


[Appstream]:        https://freedesktop.org/software/appstream/docs/chap-Quickstart.html
[CC0-1.0]:          https://creativecommons.org/publicdomain/zero/1.0/
[CI]:               https://github.com/qtilities/sqeleton/actions/workflows/build.yml/badge.svg
[Desktop Entry]:    https://specifications.freedesktop.org/desktop-entry-spec/latest/ar01s02.html
[LXQt build tools]: https://github.com/lxqt/lxqt-build-tools
[MIT]:              COPYING
[Openclipart]:      https://openclipart.org/
[website page]:     https://qtilities.github.io/sqeleton/
