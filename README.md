# GummyConf

[![CI]](https://github.com/qtilities/gummy-conf/actions/workflows/build.yml)

Configuration tool for [gummy], screen brightness/temperature manager for Linux.

## Dependencies

### Runtime

- Qt5/6 base

### Build

- CMake
- Qt Linguist Tools
- Git (optional, to pull latest VCS checkouts)

## Build

`CMAKE_BUILD_TYPE` is usually set to `Release`, though `None` might be a valid [alternative].<br />
`CMAKE_INSTALL_PREFIX` has to be set to `/usr` on most operating systems.<br />
Using `sudo make install` is discouraged, instead use the system package manager where possible.

```bash
cmake -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr -W no-dev
cmake --build build --verbose
DESTDIR="$(pwd)/package" cmake --install build
```

## Packages

[![Packaging status]](https://repology.org/project/gummy-conf/versions)

## Licenses

- GummyConf is licensed under the [MIT] license.
- Application icon is from [Openclipart], [CC0-1.0] license.


[alternative]:      https://wiki.archlinux.org/title/CMake_package_guidelines#Fixing_the_automatic_optimization_flag_override
[CC0-1.0]:          https://creativecommons.org/publicdomain/zero/1.0/
[CI]:               https://github.com/qtilities/gummy-conf/actions/workflows/build.yml/badge.svg
[gummy]:            https://github.com/f-fusco/gummy/
[MIT]:              COPYING
[Openclipart]:      https://openclipart.org/
[Packaging status]: https://repology.org/badge/vertical-allrepos/gummy-conf.svg
