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
 * file: src/starters/commands/group-objects.cpp
 * description:
 *    This plugin command groups all selected objects under a
 *    single Null object.
 * tags: command simple muchdoc group object-creation hierarchy-modifications
 * level: beginner
 * read-before: create-cube.cpp
 */

#include <c4d.h>

const static Int32 ID_GROUPOBJECTS = 1031055;

class GroupObjectsCommand : public CommandData {

public:

    //| CommandData Overrides

    virtual Bool Execute(BaseDocument* doc);

};

Bool Register_Starters_Command_GroupObjects() {
    String help_string("C++ SDK Example Command Plugin: Groups the "
                       "selected objects by inserting them under a Null.");
    CommandData* plugin_command = NewObj(GroupObjectsCommand);
    if (!plugin_command) return false; // memory error

    return RegisterCommandPlugin(
            ID_GROUPOBJECTS,
            "starters/commands/Group Objects",
            PLUGINFLAG_COMMAND_HOTKEY,
            nullptr,
            help_string,
            plugin_command);
};

Bool GroupObjectsCommand::Execute(BaseDocument* doc) {
    if (!doc) return false; // better safe than sorry

    // Create an `AtomArray` which will be filled with all
    // selected objects in the document. The `AutoAlloc`
    // class manages allocation and deallocation of the
    // internal `AtomArray` object.
    AutoAlloc<AtomArray> array;
    if (!array) return false; // memory error

    // Fill the `array` with the selected objects (excluding
    // child objects as defined by the second argument).
    // Don't forget to check the SDK documentation for
    // this method.
    doc->GetActiveObjects(*array, GETACTIVEOBJECTFLAGS_0);

    // We don't want to continue if there is no selected
    // object at all. It's not a real error so we return true.
    if (array->GetCount() <= 0) return true;

    // Create a new Null object serving as the new parent
    // for the selected objects.
    BaseObject* root = BaseObject::Alloc(Onull);
    if (!root) return false; // memory error

    // Create a new undo step.
    doc->StartUndo();

    // Iterate over all objects in the array and move them
    // to the new root object.
    Int32 count = array->GetCount();
    for (Int32 i=0; i < count; i++) {
        // Obtain the object at the current index. It is
        // guaranteed to be an object from the Objects
        // Manager so it is safe to perform the cast.
        BaseObject* obj = static_cast<BaseObject*>(array->GetIndex(i));

        // Tell the document we are about to perform some
        // hierarchy changes to the object.
        doc->AddUndo(UNDOTYPE_CHANGE, obj);

        // Remove the object from the document and insert it
        // under the root.
        obj->Remove();
        obj->InsertUnderLast(root);
    }

    // Insert the new root object into the document.
    doc->AddUndo(UNDOTYPE_NEW, root);
    doc->InsertObject(root, nullptr, nullptr);
    root->Message(MSG_MENUPREPARE);
    doc->SetActiveObject(root);
    doc->EndUndo();

    EventAdd();
    return true;
}

