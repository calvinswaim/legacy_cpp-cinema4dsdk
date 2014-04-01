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

extern Bool Register_Starters_Command_CreateCube();

/**
 * Invokes all registration functions from the starters module of
 * the Cinema 4D Plugin SDK. This is reduce the complexity of the
 * `src/main.cpp` file.
 */
Bool Register_Starters() {
    if (!Register_Starters_Command_CreateCube()) {
        GePrint("DEBUG: Register_Starters_Command_CreateCube() failed");
    }
    return true;
}

