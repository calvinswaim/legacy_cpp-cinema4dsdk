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
 */

#ifndef CINEMA4DSDK_CUSTOMGUI_FLOATLIST_H
#define CINEMA4DSDK_CUSTOMGUI_FLOATLIST_H

#include <c4d.h>

/**
 * This is the Plugin ID of the custom data type.
 */
static const Int32 CUSTOMDATATYPE_FLOATLIST = 1031955;
static const Int32 CUSTOMGUI_FLOATLIST = 1031955;

/**
 * This class reflects the data that is being stored by the
 * custom data type and modifiable by the custom GUI. We will
 * store a list of String:Float pairs in it.
 *
 * It is the same as the SplineData or the PriorityData but
 * specialized for our new custom GUI.
 */
class FloatlistData : public CustomDataType {

public:
    struct Item {
        String name;
        Float value;
    };

private:
    maxon::BaseArray<Item> items;

public:

    FloatlistData() : items() { }

    FloatlistData(const FloatlistData& other) : items() {
        items.CopyFrom(other.items);
    }

    FloatlistData& operator = (const FloatlistData& other) {
        items.Flush();
        items.CopyFrom(other.items);
        return *this;
    }

    Item& operator [] (Int32 i) {
        return items[i];
    }

    const Item& operator [] (Int32 i) const {
        return items[i];
    }

    Item* Append() {
        return items.Append();
    }

    Bool Pop() {
        return items.Pop();
    }

    void Erase(Int32 index) {
        items.Erase(index);
    }

    Int32 GetCount() const {
        return items.GetCount();
    }

    void Flush() {
        items.Flush();
    }

    void CopyFrom(const FloatlistData& other) {
        items.CopyFrom(other.items);
    }

    void CopyTo(FloatlistData& other) const {
        other.items.CopyFrom(items);
    }

    static FloatlistData* Alloc() {
        return NewObj(FloatlistData);
    }

    static void Free(FloatlistData* ptr) { // todo: add &
        if (ptr == nullptr)
            DebugStop();
        DeleteObj(ptr);
    }

    static FloatlistData* Get(const GeData& data) {
        auto temp = data.GetCustomDataType(CUSTOMDATATYPE_FLOATLIST);
        return static_cast<FloatlistData*>(temp);
    }

};

#endif /* CINEMA4DSDK_CUSTOMGUI_FLOATLIST_H */