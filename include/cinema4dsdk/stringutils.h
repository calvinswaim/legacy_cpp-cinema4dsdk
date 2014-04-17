/**
 * Copyright (c) 2014  Niklas Rosenstein
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
 * file: cinema4dsdk/stringutils.cpp
 */

#ifndef CINEMA4DSDK_STRINGUTILS_H
#define CINEMA4DSDK_STRINGUTILS_H

#include <c4d.h>

inline String ToString(const Int32 value) {
    return String::IntToString(value);
}

inline String ToString(const Float value) {
    return String::FloatToString(value);
}

inline String ToString(const Vector& v) {
    String result = "Vector(" + ToString(v.x) + ", " + ToString(v.y);
    return result + ", " + ToString(v.z) + ")";
}

inline String ToString(const BaseList2D* node) {
    if (node == nullptr)
        return "nullptr";
    else
        return node->GetName() + "(" + ToString(node->GetType()) + ")";
}

inline String ToString(const DescID& descid) {
    String result = "DescID(";
    Int32 depth = descid.GetDepth();
    for (Int32 i=0; i < depth; i++) {
        result += ToString(descid[i].id);
        if (i != (depth - 1))
            result += ", ";
    }
    return result + ")";
}

#endif /* CINEMA4DSDK_STRINGUTILS_H */