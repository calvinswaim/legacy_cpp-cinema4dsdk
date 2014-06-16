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
 * description: This plugin command demonstrates how to access points
 *    of spline or polygon objects. It positions a sphere primitive
 *    object on each selected point of an object.
 * tags: command simple muchdoc point-object selections undos
 * level: beginner
 * read-before: create-cube.cpp group-objects.cpp
 */

#include <c4d.h>

static const Int32 PLUGIN_ID = 1031057;

class SpheresOnPointsCommand : public CommandData {

public:

    //| CommandData Overrides

    // Called when the user invoked the command from the
    // plugins menu.
    virtual Bool Execute(BaseDocument* doc);

    // Called when the plugin menu is displayed. Returns
    // the state of the command. Can it be clicked? Is it
    // enabled (showing a checkmark)?
    virtual Int32 GetState(BaseDocument* doc);

};

Bool Register_Starter_Command_SpheresOnPoints() {
    String help_string("C++ SDK Example Command Plugin: Generates spheres on "
                       "the selected points of an object. Works with "
                       "point/polygon objects only.");
    CommandData* plugin_command = NewObj(SpheresOnPointsCommand);
    if (!plugin_command) return false;

    return RegisterCommandPlugin(
            PLUGIN_ID,
            "starters/commands/Spheres on Points",
            PLUGINFLAG_COMMAND_HOTKEY,
            nullptr,
            help_string,
            plugin_command);
}

Bool SpheresOnPointsCommand::Execute(BaseDocument* doc) {
    if (!doc) return false;

    // Get the active object and check if we can really work
    // with it. And as long as we do not call PointObject
    // specific methods on the object, the cast is fine.
    PointObject* op = static_cast<PointObject*>(doc->GetActiveObject());
    if (!op || !op->IsInstanceOf(Opoint)) return true; // only a user error

    // Ok we have a point object. Now we need to read to
    // retreive its point count, point positions.
    const Int32 count = op->GetPointCount();
    const Vector* points = op->GetPointR();
    if (count <= 0 || !points) return false; // application error

    // Retrieve the point selection and the number of selected
    // points.
    const BaseSelect* selection = op->GetPointS();
    Int32 selcount = selection ? selection->GetCount() : 0;

    Bool success = true;

    // Start a new undo step and iterate over all points in
    // the object.
    doc->StartUndo();
    for (Int32 i=0; i < count; i++) {
        // We only want to create spheres on selected points.
        // However, if no point is selected at all, we will use
        // all points.
        Bool is_selected = selcount <= 0 || selection->IsSelected(i);
        if (is_selected) {
            // Allocate a new sphere primitive.
            BaseObject* sphere = BaseObject::Alloc(Osphere);
            if (!sphere) {
                // Memory error, break out of the loop.
                success = false;
                break;
            }

            // Insert the sphere under the selected object.
            doc->AddUndo(UNDOTYPE_NEW, sphere);
            sphere->InsertUnder(op);

            // And move it to the points' position.
            Matrix matrix;
            matrix.off = points[i];
            sphere->SetMl(matrix);

            // Send a message to the object that it was added
            // to the document just recently. (Info: the sphere
            // adds a Phong Tag to itself on this message, the
            // same applies for many other primitive objects)
            // Generally, message should always be sent when
            // inserting a new object.
            sphere->Message(MSG_MENUPREPARE);
        }
    }

    // Tear down everything.
    doc->EndUndo();
    EventAdd();
    return success;
}

Int32 SpheresOnPointsCommand::GetState(BaseDocument* doc) {
    if (!doc) return 0;

    // We only want the command to be clickable when there
    // is a point object selected.
    BaseObject* op = doc->GetActiveObject();
    if (!op || !op->IsInstanceOf(Opoint)) return 0;

    return CMD_ENABLED;
}

