/**
 * Copyright (c) 2013-2014  Niklas Rosenstein
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
 * file: cpp-cinema4dsdk/src/starters/commands/iter-hierarchy.cpp
 * description: This plugin command demonstrates how to recursively
 *    iterate over a documents object hierarchy and print them to
 *    the console.
 * tags: command simple muchdoc hierarchy-iteration recursion console
 * level: beginner
 * read-before: create-cube.cpp group-objects.cpp
 */

#include <c4d.h>

static const Int32 PLUGIN_ID = 1031056;

class IterHierarchyCommand : public CommandData {

public:

    //| CommandData Overrides

    virtual Bool Execute(BaseDocument* doc);

};

Bool Register_Starter_Command_IterHierarchy() {
    String help_string("C++ SDK Example Command Plugin: Demonstrates "
                       "working through the object tree recursively and "
                       "printing the hierarchly structure to the console.");
    CommandData* plugin_command = NewObj(IterHierarchyCommand);
    if (!plugin_command) return false; // memory error

    return RegisterCommandPlugin(
            PLUGIN_ID,
            "starters/commands/Iter Hierarchy",
            PLUGINFLAG_COMMAND_HOTKEY,
            nullptr,
            help_string,
            plugin_command);
}

void ApplyRecursion(BaseObject* op, Int32 depth) {
    // Create the indentation from the `depth` parameter.
    String line;
    for (Int32 i=0; i < depth; i++) line += "    ";

    // Add the objects' name to the line nd print it to the
    // Cinema 4D console.
    line += "- " + op->GetName();
    GePrint(line);

    // Iterate over all child objects of the current object.
    BaseObject* child = op->GetDown();
    while (child) {
        // Call the function with the child again.
        ApplyRecursion(child, depth + 1);
        child = child->GetNext();
    }
}

Bool IterHierarchyCommand::Execute(BaseDocument* doc) {
    if (!doc) return false; // better safe than sorry

    // Get the first object from document and iterate
    // over all following objects in the same hierarchy.
    BaseObject* obj = doc->GetFirstObject();
    while (obj) {
        ApplyRecursion(obj, 0);
        obj = obj->GetNext();
    }

    // Open the Cinema 4D console by invoking the command
    // plugin of it.
    CallCommand(12305); // console plugin ID, see C4D Script Log
    return true;
}

