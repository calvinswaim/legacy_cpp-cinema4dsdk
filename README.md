# C++ Cinema 4D SDK

This repository is the new community powered Cinema 4D SDK. Download,
compile and you are ready to learn the Cinema 4D Plugin API from these
examples. \*

_\* that is our goal, we are just starting out with this project._

__Minimum Requirements__: MAXON introduced major name changes to its
Plugin API with the release of Cinema 4D R15. The code in this
repository is written for this new naming scheme and is not backwards
compatible. __However__, since no semantics changed, the code can
be read by developers used to prior versions of the API without
problems.

__What this repository consists of__: Most of the examples in this
repository will be initiated by questions at the PluginCafé forums
where full examples where requested to understand a certain part of
the Plugin API. __Feel free__ to fork this repository and issue a pull
request if you want to contribute.

__License__: This source code in this repository is, if not declared
otherwise, licensed under the MIT license.

## Structure

There is a strict structure to this repository. All __project files__
belonging to compilation tools such as Visual Studio, XCode or
Makefiles are located in the `projects/` directory. Compilation tools
should output __object files__ to the `obj/` directory while the build
results are the `cinema4dsdk.cdl64` or `cinema4dsdk.dylib`, respective
to the target platform.

All __source files__ (ie. `*.cpp` files) are contained in the `src/`
directory while __includable files__ are contained in the `include/`
directory. The Cinema 4D plugin __resource folder__ is called `res/`
which should also contain any image and other required files.

## Compile

The project files to compile the Cinema 4D SDK are located in the
`projects/` folder. To compile with [`c4d-make`][c4d-make], invoke

    $ pwd
    <...>/Cinema 4D R15/plugins/cpp-cinema4dsdk
    $ make -f projects/c4d-make/Makefile <target>

Open the project files for Visual Studio or XCode respectively if
you want to use any of these.

## Todo

- Add Visual Studio project files
- Add XCode project files


  [c4d-make]: https://github.com/nr-tools/c4d-make
