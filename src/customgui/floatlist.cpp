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
 * file: src/customgui/floatlist.cpp
 */

#include <c4d.h>
#include <cinema4dsdk/stringutils.h>
#include <cinema4dsdk/customgui/floatlist.h>


static String ToString(const FloatlistData& data, Bool detailed=false) {
    String result = "FloatlistData(" + ToString(data.GetCount()) + ")";
    if (detailed) {
        for (Int32 i=0; i < data.GetCount(); i++) {
            const auto& item = data[i];
            result += "\n  #" + ToString(i) + ": " + item.name;
            result += " = " + ToString(item.value);
        }
    }
    return result;
}

static String ToString(const FloatlistData* data, Bool detailed=false) {
    if (data == nullptr)
        return "FloatlistData(nullptr)";
    return ToString(*data, detailed);
}


/**
 * This class implements how Cinema 4D is supposed to treat our
 * shiny new custom data type. For instance, if you pipe the data
 * into a float input port with XPresso, what value should it
 * receive? Or should it not be actually allowed? This is what
 * this class handles.
 */
class FloatlistDataType : public CustomDataTypeClass {

    typedef CustomDataTypeClass super;

public:

    static Bool Register() {
        auto data = NewObj(FloatlistDataType);
        Int32 flags = CUSTOMDATATYPE_INFO_HASSUBDESCRIPTION | CUSTOMDATATYPE_INFO_NEEDDATAFORSUBDESC;
        return RegisterCustomDataTypePlugin("Floatlist", flags, data, 1000);
    }

    // CustomDataTypeClass

    virtual Int32 GetId() {
        return ::CUSTOMDATATYPE_FLOATLIST;
    }

    virtual CustomDataType* AllocData() {
        return NewObj(FloatlistData);
    }

    virtual void FreeData(CustomDataType* data) {
        if (data != nullptr)
            FloatlistData::Free(static_cast<FloatlistData*>(data));
    }

    virtual Bool CopyData(const CustomDataType* src_, CustomDataType* dst_,
                AliasTrans* trans)
    {

        auto src = static_cast<const FloatlistData*>(src_);
        auto dst = static_cast<FloatlistData*>(dst_);
        if (src == nullptr || dst == nullptr)
            return false;

        dst->CopyFrom(*src);
        return true;
    }

    virtual Int32 Compare(const CustomDataType* a_, const CustomDataType* b_) {

        auto a = static_cast<const FloatlistData*>(a_);
        auto b = static_cast<const FloatlistData*>(b_);
        if (a == nullptr || b == nullptr)
            return -1; // lower than

        // There is not really a good and easy way to compare two
        // lists as we have here, so we just go by their length.

        Int32 countA = a->GetCount();
        Int32 countB = b->GetCount();
        if (countA == countB) {
            // Important Fact: When the value from the Custom GUI is
            // updated, Cinema first checks if the values equal. If they
            // do, the original value will be kept. So, if we just say
            // to Floatlists are equal when they have the same length,
            // the actual value of each item won't be updated because
            // the original Floatlist is kept.
            //
            // Conclusion: Compare thorougly!
            //
            // Since there is no value for just "unequal", we say
            // if the Floatlists don't equal but we can not tell
            // if the one is lower or greater, we just use lower.

            static const Float ELLIPSIS = 0.00001;
            for (Int32 i=0; i < countA; i++) {
                const auto& itemA = (*a)[i];
                const auto& itemB = (*b)[i];

                if (itemA.name != itemB.name)
                    return -1; // lower than
                if (Abs(itemA.value - itemB.value) > ELLIPSIS)
                    return -1; // lower than
            }

            return 0; // equal
        }
        else if (countA < countB)
            return -1; // lower than
        else
            return 1; // greater than
    }

    virtual Bool WriteData(const CustomDataType* data_, HyperFile* hf) {

        auto data = static_cast<const FloatlistData*>(data_);
        if (data == nullptr)
            return false;

        // We first write the number of items, and then each item
        // separately to the HyperFile.

        Int32 count = data->GetCount();
        if (!hf->WriteInt32(count)) return false;

        for (Int32 index=0; index < count; index++) {
            const auto& item = (*data)[index];
            if (!hf->WriteString(item.name)) return false;
            if (!hf->WriteFloat(item.value)) return false;
        }

        return true;
    }

    virtual Bool ReadData(CustomDataType* data_, HyperFile* hf, Int32 level)
    {

        auto data = static_cast<FloatlistData*>(data_);
        if (data == nullptr)
            return false;

        // We need to read the data the way we have previously
        // written it to the HyperFile. If any of the read methods
        // fails, the input data is invalid (eg. the file is corrup).

        Int32 count;
        if (!hf->ReadInt32(&count)) return false;

        // Clear all elements in the array so we start from an
        // empty state.
        data->Flush();
        for (Int32 index=0; index < count; index++) {

            // Create a new item at the end of the BaseArray.
            auto item = data->Append();
            if (item == nullptr) return false;

            // Read the name and value from the file into the
            // members of the created item.
            if (!hf->ReadString(&item->name)) return false;
            if (!hf->ReadFloat(&item->value)) return false;

        }

        return true;
    }

    virtual Bool _GetDescription(const CustomDataType* data_, Description& desc,
                DESCFLAGS_DESC& flags, const BaseContainer& parentDesc,
                DescID* unused)
    {
        if (data_ == nullptr)
            return false;

        const auto& data = *static_cast<const FloatlistData*>(data_);

        // Create the default parameter description that we
        // will use for each item in the FloatlistData.
        BaseContainer default_ = GetCustomDataTypeDefault(DA_REAL);
        default_.SetFloat(DESC_MIN, 0.0);
        default_.SetFloat(DESC_MAX, 1.0);
        default_.SetFloat(DESC_STEP, 0.01);
        default_.SetInt32(DESC_UNIT, DESC_UNIT_PERCENT);
        default_.SetInt32(DESC_CUSTOMGUI, CUSTOMGUI_REALSLIDER);

        // todo: The parameters are not accessible via XPresso.

        // Create a parameter for each entry in the FloatlistData.
        Int32 count = data.GetCount();
        for (Int32 i=0; i < count; i++) {
            const auto& item = data[i];
            default_.SetString(DESC_NAME, item.name);
            default_.SetString(DESC_SHORT_NAME, item.name);

            // Note: DescID(0) doesn't work, we'll make it similar to the
            // Vector DescIDs and start with 1000 (it's common convention
            // to start with 100 in the Cinema 4D Plugin API)
            desc.SetParameter(DescID(1000 + i), default_, DESCID_ROOT);
        }

        // Tell that we loaded the description.
        flags |= DESCFLAGS_DESC_LOADED;
        return super::_GetDescription(data_, desc, flags, parentDesc, unused);
    }

    virtual Bool GetParameter(const CustomDataType* data_, const DescID& id,
                GeData& dest, DESCFLAGS_GET& flags)
    {
        if (data_ == nullptr)
            return false;

        const auto& data = *static_cast<const FloatlistData*>(data_);

        // Don't forget we added 1000 in _GetDescription(), so we
        // subtract it again to get the actual index of the item that
        // is about to be retreived.
        Int32 index = id[0].id - 1000;

        Int32 count = data.GetCount();
        if (index >= 0 && index < count) {
            const auto& item = data[index];
            dest.SetFloat(item.value);

            // Tell that the parameter could be retreived successfully.
            flags |= DESCFLAGS_GET_PARAM_GET;
            return true;
        }

        return super::GetParameter(data_, id, dest, flags);
    }

    virtual Bool SetDParameter(CustomDataType* data_, const DescID& id,
                const GeData& value, DESCFLAGS_SET& flags)
    {
        if (data_ == nullptr)
            return false;

        auto& data = *static_cast<FloatlistData*>(data_);

        // Same as in GetParameter(), we have to keep in mind that
        // the Description ID is offset by 1000 for the actual index
        // in the FloatlistData.
        Int32 index = id[0].id - 1000;

        Int32 count = data.GetCount();
        if (index >= 0 && index < count) {
            auto& item = data[index];
            item.value = value.GetFloat();

            // Tell that the parameter could be set successfully.
            flags |= DESCFLAGS_SET_PARAM_SET;
            return true;
        }

        return super::SetDParameter(data_, id, value, flags);
    }

    virtual const Char* GetResourceSym() {
        // This should return the name of the custom data type
        // as it is used in resource files.
        return "FLOATLIST";
    }

    virtual void GetDefaultProperties(BaseContainer& data) {
        data.SetInt32(DESC_ANIMATE, DESC_ANIMATE_OFF);
        data.SetInt32(DESC_CUSTOMGUI, CUSTOMGUI_FLOATLIST);
    }

};

/**
 * This class manages the displaying of the custom data type
 * to the user. It is a GeDialog that is attached in the AM
 * or to a dialog.
 */
class FloatlistGui : public iCustomGui {

    typedef iCustomGui super;

    /**
     * This enumeration declares the widget IDs used in this dialog
     * implementation.
     */
    enum {
        GROUP_MAIN = 20000,
        TEXT_MULTIPLE,
        BUTTON_PLUS,
        BUTTON_MINUS,

        // The start ID for the dynamic widgets.
        DYNAMIC_START,

        // A shorthand for the full scalefit flag.
        HV_SCALEFIT = BFH_SCALEFIT | BFV_SCALEFIT,
    };

    /**
     * Indicates that the UI is supposed to display multiple values
     * and that therefore no accurate representation is possible. This
     * member is updated in SetData() and used in Rebuild().
     */
    Bool m_multiple;

    /**
     * This member stores the number of elements that have been
     * displayed the last time the dialog was updated. Using this
     * information, we can rebuild the UI when necessary (that is,
     * when the number of items in the Floatlist changed).
     */
    Int32 m_count;

    /**
     * This is just a copy of the data that is used when signaling
     * the parent that the value has changed (we don't want to loose
     * the names in the Floatlist structure).
     */
    FloatlistData m_data;

public:

    FloatlistGui(const BaseContainer& settings, CUSTOMGUIPLUGIN* plugin)
    : super(settings, plugin),
      m_multiple(false),
      m_count(-2)
    { }

    /**
     * Informs the parent about the new value the custom GUI
     * created by user input.
     */
    inline Bool SendValueChanged(BaseContainer msg, const GeData& value) {
        msg.SetInt32(BFM_ACTION_ID, GetId());
        msg.RemoveData(BFM_ACTION_VALUE);
        msg.SetData(BFM_ACTION_VALUE, value);
        return super::SendParentMessage(msg);
    }

    /**
     * Rebuilds the UI of the dialog.
     */
    void Rebuild(const FloatlistData* data) {
        // If we display the actual item but the count didn't change,
        // we do not have to rebuild.
        if (!m_multiple && data->GetCount() == m_count)
            return;

        // If we display the message that multiple values are fed
        // into the custom GUI, we don't have to rebuild if the
        // count was reset to -1.
        else if (m_multiple && m_count < 0)
            return;

        // Flush the dialog and re-create the main group.
        LayoutFlushGroup(0);
        GroupBegin(GROUP_MAIN, HV_SCALEFIT, (m_multiple ? 1 : 2), 0, "", 0);
        GroupBorderNoTitle(BORDER_ROUND);
        GroupBorderSpace(4, 4, 4, 4);

        if (m_multiple) {
            AddStaticText(TEXT_MULTIPLE, BFH_LEFT, 0, 0, "<<< Multiple Values >>>", 0);
            GroupEnd();

            m_count = -1;
        }
        else {
            // Create text and sliders for each item in the data.
            m_count = data->GetCount();
            Int32 id = DYNAMIC_START;
            for (Int32 index=0; index < m_count; index++) {
                auto& item = (*data)[index];
                AddStaticText(id++, BFH_LEFT, 0, 0, item.name, 0);
                AddEditSlider(id++, BFH_SCALEFIT);
            }

            GroupEnd();

            // Add Buttons to add and remove items.
            GroupBegin(0, 0, 0, 1, "", 0);
            AddButton(BUTTON_PLUS, 0, 0, 0, "+");
            AddButton(BUTTON_MINUS, 0, 0, 0, "-");
            GroupEnd();

            Enable(BUTTON_MINUS, m_count > 0);
        }

        LayoutChanged(0);
    }

    /**
     * Updates the values in the Dialog.
     */
    void Update(const FloatlistData* data) {
        if (m_multiple)
            return;
        DebugAssert(data != nullptr);

        Int32 count = data->GetCount();
        for (Int32 i=0; i < count; i++) {

            const auto& item = (*data)[i];

            // Calculate the ID of the slider for the current
            // element of the Floatlist and set the value to
            // the dialog.
            Int32 id = DYNAMIC_START + i * 2 + 1;
            SetPercent(id, item.value);
        }

        m_data = *data;
    }

    // iCustomGui

    virtual Bool SetData(const TriState<GeData>& tristate) {
        FloatlistData* temp = nullptr;

        // GetTri() returns true if any of the values added
        // to the tristate differed and a special tristate has
        // to be displayed in the UI.
        if (m_multiple && tristate.GetTri()) {
            // Nothing to do since we are still displaying multiple
            // values as info text in the dialog.
            return true;
        }

        m_multiple = tristate.GetTri();
        if (!m_multiple) temp = FloatlistData::Get(tristate.GetValue());
        if (!temp) {
            m_multiple = true;
        }

        Rebuild(temp);
        Update(temp);

        return true;
    }

    // GeDialog

    virtual Bool CreateLayout() {
        return true;
    }

    virtual Bool Command(Int32 id, const BaseContainer& msg) {
        String name;
        Bool updateValue = false;
        Int32 count = m_data.GetCount();

        switch (id) {
            case BUTTON_PLUS:
                if (RenameDialog(&name)) {
                    auto item = m_data.Append();
                    if (item != nullptr) {
                        item->name = name;
                        item->value = 42.0;
                        updateValue = true;
                    }
                }
                break;
            case BUTTON_MINUS:
                if (m_data.GetCount() > 0) {
                    m_data.Pop();
                    updateValue = true;
                }
                break;
        }

        // Check if the changed parameter is one of the sliders
        // and update the item in that case.
        if (id >= DYNAMIC_START && id < DYNAMIC_START + count * 2) {
            Int32 index = (id - DYNAMIC_START) / 2;
            if (index >= 0 && index < count) {

                // Get the value of the  slider.
                Float value;
                GetFloat(id, value);

                // Assign it to the item and make sure the parent
                // is notified about the changed data.
                auto& item = m_data[index];
                item.value = value;
                updateValue = true;
            }
        }

        // Update the value stored by the holder of the Custom GUI (ie.
        // the node or the dialog value) if that is requested.
        if (updateValue) {
            GeData data(CUSTOMDATATYPE_FLOATLIST, m_data);
            SendValueChanged(msg, data);
        }
        return true;
    }

};

/**
 * This class manages the custom GUI allocation and deallocation.
 * It also declares the resource symbol which will be used in
 * dialog resources and descriptions.
 */
class FloatlistGuiData : public CustomGuiData {

public:

    static Bool Register() {
        // Register a fake library as it is shown in the
        // documentation. This is required for the custom GUI
        // to work.
        static BaseCustomGuiLib lib;
        ClearMem(&lib, sizeof(lib));
        FillBaseCustomGui(lib);
        if (!InstallLibrary(CUSTOMGUI_FLOATLIST, &lib, 1000, sizeof(lib)))
            return false;

        // Create a new instance of the plugin type.
        auto data = NewObj(FloatlistGuiData);

        if (!RegisterCustomGuiPlugin("Floatlist", 0, data)) {
            DeleteObj(data);
            return false;
        }
        return true;
    }

    // CustomGuiData

    virtual Int32 GetId() {
        return CUSTOMGUI_FLOATLIST;
    }

    virtual CDialog* Alloc(const BaseContainer& settings) {
        // Allocate the new FloatlistGui object with the supplied
        // settings.
        auto dialog = NewObj(FloatlistGui, settings, this->GetPlugin());
        if (dialog != nullptr)
            return dialog->Get();
        return nullptr;
    }

    virtual void Free(CDialog* dialog_, void* userdata) {
        if (dialog_ == nullptr)
            return;

        // Delete the dialog that we allocated in Alloc(). The
        // documentation suggests converting it to the original
        // type of the dialog, must be some relic from the older
        // API versions.
        auto dialog = reinterpret_cast<FloatlistGui*>(dialog_);
        DeleteObj(dialog);
    }

    virtual const Char* GetResourceSym() {
        // Same as in FloatlistDataType::GetResourceSym(), but the
        // name here is used in the resource as  CUSTOMGUI FLOATLIST;
        return "FLOATLIST";
    }

    virtual Int32 GetResourceDataType(Int32*& table) {
        // Report the IDs of the data types that this GUI is
        // compatible with. We are only compatible with the
        // floatlist data type.
        static Int32 ids[] = {
            CUSTOMDATATYPE_FLOATLIST,
        };

        table = ids;
        return 1;
    }

};


/**
 * Called from src/main.cpp on PluginStart(). This function registers
 * the Floatlist custom datatype and GUI to Cinema 4D.
 */
Bool Register_FloatlistCustomGui() {
    if (!FloatlistDataType::Register()) {
        GePrint("DEBUG: Could not register FloatlistDataType");
        return false;
    }
    if (!FloatlistGuiData::Register()) {
        GePrint("DEBUG: Could not register FloatlistGuiData");
        return false;
    }
    return true;
}

