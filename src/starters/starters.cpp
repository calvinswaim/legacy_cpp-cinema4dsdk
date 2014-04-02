/**
 * Copyright (c) 2013-2014 Niklas Rosenstein
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * file: src/starters/starters.cpp
 * description: registers all plugin examples for starters of the Cinema
 *     4D Plugin SDK.
 */

#include <c4d.h>

/**
 * A small makro that is used to easily call a register function from
 * another compiled object file that starts with the name
 * `Register_Starters_`.
 */
#define REGISTER(name) \
    do { \
        extern Bool Register_Starters_##name(); \
        if (!Register_Starters_##name()) { \
            GePrint("DEBUG: Register_Starters_##name() failed"); \
        } \
    } while (0)

/**
 * Invokes all registration functions from the starters module of
 * the Cinema 4D Plugin SDK. This is reduce the complexity of the
 * `src/main.cpp` file.
 */
Bool Register_Starters() {
    REGISTER(Command_CreateCube);
    REGISTER(Command_GroupObjects);
    REGISTER(Command_IterHierarchy);
    return true;
}

