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

## New to the Cinema 4D plugin development?

Take a look at the [starters/][] folder, it contains plugin examples
that are suited for very beginners. Having experience in programming
C++ code however is of very much advantage.

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

You may also want to look at the readme file that is available for
many source files. These have the same name but the Markdown suffix 
(`*.md`) and provide additional information to the example.

## File Headers

Besides the license text, the documentation headers of source files
contain useful information such as a short description, the experience
level required to understand the example, tags and names of files that
should be read and understand before reading that example file.

Take the `src/starters/commands/group-objects.cpp` file for an instance:

```cpp
/**
 * Copyright (c) 2013-2014  Niklas Rosenstein
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * [...]
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * file: src/starters/commands/group-objects.cpp
 * description:
 *    This plugin command groups all selected objects under a
 *    single Null object.
 * tags: command simple muchdoc group object-creation hierarchy-modifications
 * level: beginner
 * read-before: create-cube.cpp
 */
```

__file__: The full path of the file in the Cinema4D SDK project relative
to the projects main directory.

__description__: A short description of the plugin. Some files might have
an `*.md` file in the same directory with the same name which contains a
more detailed description of the example.

__tags__: A list of the tags that match with the example.

__level__: The level someone requires to understand the example.

__read-before__: The name of one or more files that should be read and
understood before reading the current file.

## Compile

The project files to compile the Cinema 4D SDK are located in the
`projects/` folder. To compile with [`c4d-make`][c4d-make], invoke

    $ pwd
    <...>/Cinema 4D R15/plugins/cpp-cinema4dsdk
    $ make -f projects/c4d-make/Makefile plugin

Open the project files for Visual Studio or XCode respectively if
you want to use any of these.

> *Note*: The project files for Visual Studio and XCode are not yet ready.

## Todo

- Add Visual Studio project files
- Add XCode project files


  [c4d-make]: https://github.com/nr-tools/c4d-make
