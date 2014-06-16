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

__License__: This source code in this repository, if not declared
otherwise in the file header, is licensed under the MIT license. Coypright
is granted to the original author of the source file.

## New to the Cinema 4D plugin development?

Take a look at the [starters/][] folder, it contains plugin examples
that are suited for very beginners. Having experience in programming
C++ code however is of very much advantage.

## Structure

There is a strict structure to this repository. All _project files_
belonging to compilation tools such as Visual Studio, XCode or
Makefiles are located in the `projects/` directory.

You may also want to look at the readme file that is available for
some of the source files. These have the same name as the source file
but with Markdown suffix (`*.md`) and provide additional information to
the example.

## File Headers

Besides the license text, the documentation headers of source files
contain useful information such as a short description, the experience
level required to understand the example, tags and names of files that
should be read and understand before reading that example file.

Take the `source/cinema4dsdk/starters/commands/group-objects.cpp` file
for an instance:

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
 * description:
 *    This plugin command groups all selected objects under a
 *    single Null object.
 * tags: command simple muchdoc group object-creation hierarchy-modifications
 * level: beginner
 * read-before: create-cube.cpp
 */
```

__description__: A short description of the plugin. Some files might have
an `*.md` file in the same directory with the same name which contains a
more detailed description of the example.

__tags__: A list of the tags that match with the example.

__level__: The level someone requires to understand the example.

__read-before__: The name of one or more files that should be read and
understood before reading the current file.

## Compile

> Note: The __XCode__ Project Files are not yet available.

__Visual Studio__

The Visual Studio Solution is located at `projects/visual-studio/cinema4dsdk.sln`.

__c4dmake__

The [c4dmake][] Makefile library for compiling the Cinema 4D API and
Plugins are *not officially* supported. Support is only given on personal
preference of the maintainer, [Niklas Rosenstein](https://github.com/NiklasRosenstein).

To compile using c4dmake, you must first install the Makefiles into
the Cinema 4D plugins folder as described in the [repository][c4dmake]. The
destination folder must be called `.../Cinema 4D R15/plugins/api/`. This is
a very simpley task. Please report any issues during the following steps to
the [c4dmake][] issuetracker.

    $ pwd
    <...>/Cinema 4D R15/plugins/api
    $ make lib
    $ make lib DEBUG=true

After this is complete without errors, you are __ready to compile__ the new
Cinema 4D SDK. Please report any issues during the following steps to the
issuetracker of the cpp-cinema4sdk repository.

    $ pwd
    <...>/Cinema 4D R15/plugins/cpp-cinema4dsdk
    $ make -f projects/c4dmake/Makefile plugin

## Contributions are Welcome!

Don't hesitate to fork this repository and add new stuff to it. GitHub allows
you to make a request that we will merge your changes into the main repository,
and so we will do! It would be nice if you'd stick to the File Headers information
for each file as we plan to do something with this information.

We prefer the MIT license, but if, for any special reasons, you want to put your
contributed sources under a different license, we are fine with that as long as
it does not contradict with other contents of the repository! The *copyright* is
granted to the original author of the source file, so don't forget to add it to
the file(s).

## Todo

- Add XCode project files

  [c4dmake]:   https://github.com/nr-tools/c4dmake
  [starters/]: https://github.com/PluginCafe/cpp-cinema4dsdk/tree/master/source/cinema4dsdk/starters

