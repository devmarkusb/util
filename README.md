# util library (ul)
[![build](https://github.com/devmarkusb/util/actions/workflows/build.yml/badge.svg)](https://github.com/devmarkusb/util/actions/workflows/build.yml)

A personal library serving purposes that come right
after or next to std.

For convenience, util or util library is abbreviated as
namespace ul or prefix UL_ for macros everywhere.

## What's in it?

* Above all, your entry point is browsing the header files.
* The subdirs are just for topic-wise organization - in the end
it is just one lib, accessible by include `#include "ul/ul.h"`, except
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

```
add_subdirectory(util)
# ...
target_include_directories(YourTarget PUBLIC ${ul_INCLUDE_DIRS})
```
or
```
cmake_minimum_required(VERSION 3.14)

if (TARGET ulBuildEnv)
    return ()
endif ()

include(FetchContent)

FetchContent_Declare(mb-util
        GIT_REPOSITORY "/home/markus/projects/git/libs/util"
        GIT_TAG origin/HEAD
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(mb-util)
# ...
target_include_directories(YourTarget PUBLIC ${ul_INCLUDE_DIRS})
```
and `#include "ul/ul.h"` should be all you need. The library is header-only.

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `ul`
around, you can set `UL_DISABLE_NAMESPACE_ALIAS` to `ON`
which yields base namespace `mb::ul` instead.

## Contributing

### todos

* any `//todo` comment in code
* use concepts, but don't merge as long as Android build needs old compiler
