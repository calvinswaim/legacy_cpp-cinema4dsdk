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
 * file: src/starters/commands/create-cube.cpp
 * description: This plugin command creates a Cube object and places
 *    it at the position of the selected object. If there is
 *    no active object, it is placed at the world's origin.
 * tags: command simple muchdoc object-creation
 * level: beginner
 */

// Include the Cinema 4D header files that are required
// for plugin development.
#include <c4d.h>

// Every single plugin requires a unique ID, which can be
// obtained from the plugincafe.
static const Int32 ID_CREATECUBE = 1031054;

// This class overwrites methods from the Cinema 4D SDK
// `CommandData` class to implement a custom action when
// the user selects the command from the Cinema 4D plugin
// menu.
class CreateCubeCommand : public CommandData {

public:

    //| CommandData Overrides

    // This method is called when the user selects the
    // command from the plugin's menu. The `doc` parameter
    // represents the active scene in the Cinema 4D window.
    virtual Bool Execute(BaseDocument* doc);

};

// This function takes care of registering the plugin to
// Cinema 4D. It is (and must be) called from the `PluginStart()`
// function of a plugin library. In this case, it is called from
// `RegisterStarters()` in `src/starters/starters.cpp`
Bool Register_Starters_Command_CreateCube() {
    // This string is used later for registering the plugin.
    String help_string("C++ SDK Example Command Plugin: Creates a cube and "
                       "assigns the matrix of the selected object to it.");

    // Allocate a single instance of our plugin. We need to
    // use `NewObj()`, otherwise Cinema will crash on exit
    // when it tries to free the memory.
    CommandData* plugin_command = NewObj(CreateCubeCommand);
    if (!plugin_command) return false; // memory error

    // Finally, register the plugin to Cinema 4D. :-)
    return RegisterCommandPlugin(
            ID_CREATECUBE,                    // Plugin ID,
            "starters/commands/Create Cube",  // Command Name
            PLUGINFLAG_COMMAND_HOTKEY,        // Flags (options for the plugin)
            nullptr,                          // Icon (none here)
            help_string,                      // Help string (obviously..)
            plugin_command);                  // An instance of our plugin
}

// Now we implement the method of our plugin class and
// do whatever we like.
Bool CreateCubeCommand::Execute(BaseDocument* doc) {
    if (!doc) return false; // better safe than sorry

    // Create a simple cube object.
    BaseObject* cube = BaseObject::Alloc(Ocube);
    if (!cube) return false; // memory error

    // Grab a reference to the active object from the
    // active scene.
    BaseObject* active_object = doc->GetActiveObject();

    // If there is a (single) object selected, the cube
    // should inherit it's matrix.
    if (active_object) {
        cube->SetMg(active_object->GetMg());
    }

    // We want the action to be undoable so we start
    // a new undo step.
    doc->StartUndo();

    // Now tell the document that we're about to add an
    // object, insert it and tell it, that it was added
    // to the document by sending MSG_MENUPREPARE message.
    doc->AddUndo(UNDOTYPE_NEW, cube);
    doc->InsertObject(cube, nullptr, nullptr);
    cube->Message(MSG_MENUPREPARE);

    // Make it the only active object in the scene.
    doc->SetActiveObject(cube);

    // End the undo-step, this is absolute required otherwise
    // it might mess up the undo system of the document.
    doc->EndUndo();

    // Tell Cinema to update its UI as soon as possible (to
    // reflect the change in the Object Manager and the Viewport).
    EventAdd();
    return true; // everything ok!
}

