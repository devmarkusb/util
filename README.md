# util library (ul)

[![CI status](https://github.com/devmarkusb/util/actions/workflows/ci.yml/badge.svg)](https://github.com/devmarkusb/util/actions/workflows/ci.yml)
[![clang-tidy review status](https://github.com/devmarkusb/util/actions/workflows/clang-tidy-review.yml/badge.svg)](https://github.com/devmarkusb/util/actions/workflows/clang-tidy-review.yml)
[![cppcheck status](https://github.com/devmarkusb/util/actions/workflows/cppcheck.yml/badge.svg)](https://github.com/devmarkusb/util/actions/workflows/cppcheck.yml)
[![Codacy security scan status](https://github.com/devmarkusb/util/actions/workflows/codacy.yml/badge.svg)](https://github.com/devmarkusb/util/actions/workflows/codacy.yml)
[![pre-commit check status](https://github.com/devmarkusb/util/actions/workflows/pre-commit-check.yml/badge.svg)](https://github.com/devmarkusb/util/actions/workflows/pre-commit-check.yml)

A personal library serving purposes that come right
after or next to std.

For convenience, util or util library is abbreviated as
namespace `(mb::)ul` or prefix `MB_UL_` for macros everywhere.

## What's in it?

* Above all, your entry point is browsing the header files.
* The subdirs are just for topic-wise organization - in the end
it is just one lib, accessible by include `#include "mb/ul/ul.hpp"`, except
for some macro heavy headers or exotics.
* buildenv just contains stuff helping you in setting up build
environments, no real coding support.
* basiccodesupport contains the core coding utilities. Sometimes,
parts that are getting bigger or should get a name standing out,
are shifted into other, separate parts of the lib.
* Note that, ul (buildenv) also includes/contains the gsl lib
from the cpp core guidelines. You can just use it by `#include "gsl/gsl"`.

## Usage

### CMake

```cmake
add_subdirectory(util)
# ...
target_link_libraries(YourTarget PRIVATE mb::util)
```

or

```cmake
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(mb.util
        GIT_REPOSITORY "/home/markus/projects/git/libs/util"
        GIT_TAG origin/HEAD
        GIT_SHALLOW  ON
        GIT_PROGRESS ON
        )

FetchContent_MakeAvailable(mb.util)
# ...
target_link_libraries(YourTarget PRIVATE mb::util)
```

and `#include "mb/ul/ul.hpp"` should be all you need. The library is header-only.

### Local static analysis

Run the same `cppcheck` entrypoint that CI uses:

```bash
./run-cppcheck.sh
```

This configures the default `clang-release` preset, uses the generated
`compile_commands.json`, and fails on `cppcheck` findings. The underlying CMake
helper will also do a best-effort install of `cppcheck` via a supported host
package manager when it is missing. Pass a preset name as the first argument to
use a different configure preset.

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `ul`
around, you can set `MB_UL_DISABLE_NAMESPACE_ALIAS` to `ON`
which yields base namespace `mb::ul` instead.

## Contributing

### todos

* any `//todo` comment in code
* use concepts, but don't merge or omit compile guards as long as Android build
needs old compiler
* provide callback option for assert macros
* provide stacktrace for throwing assert
