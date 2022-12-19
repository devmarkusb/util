# util library (ul)

A personal library serving purposes that come right
after or next to std.

Practically util or util library is abbreviated as
namespace ul or prefix UL_ everywhere.

## What's in it?

* Above all your entry point is browsing the header files.
* buildenv just contains stuff helping you in setting up build
environments, no real coding support.
* basiccodesupport contains the core coding utilities, sometimes
parts that are getting bigger or should get a name standing out,
are shifted into other, separate parts of the lib.
* Note that, ul (buildenv) also includes/contains the gsl lib
from the cpp core guidelines. You can just use it by `#include "gsl/gsl"`.

## Usage

### Cmake

```
add_subdirectory(util)
# ...
target_include_directories(YourTarget PUBLIC ${ul_INCLUDE_DIRS})
```
should be all you need.
If a part of the lib requires static linking, you will get to this
automatically, or via a local README.

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `ul`
around, you can do something like
`add_compile_definitions(UL_DISABLE_NAMESPACE_ALIAS=1)`.

## Contributing

* In case you're wondering, file preambles like `// 2015-17` are the
only remainder of old 'Copyright' notices. They denote the year of
creation up to the year of the latest change.

