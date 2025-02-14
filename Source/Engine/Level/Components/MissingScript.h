// Copyright (c) 2012-2023 Wojciech Figat. All rights reserved.

#pragma once

#if USE_EDITOR

#include "Engine/Core/Cache.h"
#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/ScriptingObjectReference.h"
#include "Engine/Serialization/JsonWriters.h"

/// <summary>
/// Actor script component that represents missing script.
/// </summary>
API_CLASS(Attributes="HideInEditor") class FLAXENGINE_API MissingScript : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(MissingScript);

private:
    ScriptingObjectReference<Script> _referenceScript;

public:
    /// <summary>
    /// Namespace and type name of missing script.
    /// </summary>
    API_FIELD(Attributes="ReadOnly") String MissingTypeName;

    /// <summary>
    /// Missing script serialized data.
    /// </summary>
    API_FIELD(Hidden, Attributes="HideInEditor") String Data;

    /// <summary>
    /// Field for assigning new script to transfer data to.
    /// </summary>
    API_PROPERTY() ScriptingObjectReference<Script> GetReferenceScript() const
    {
        return _referenceScript;
    }

    /// <summary>
    /// Field for assigning new script to transfer data to.
    /// </summary>
    API_PROPERTY() void SetReferenceScript(const ScriptingObjectReference<Script>& value)
    {
        _referenceScript = value;
        if (Data.IsEmpty())
            return;
        rapidjson_flax::Document document;
        document.Parse(Data.ToStringAnsi().GetText());

        auto modifier = Cache::ISerializeModifier.Get();
        _referenceScript->Deserialize(document, modifier.Value);

        DeleteObject();
    }
};

inline MissingScript::MissingScript(const SpawnParams& params)
    : Script(params)
{
}

#endif
