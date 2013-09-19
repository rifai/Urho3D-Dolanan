//
// Copyright (c) 2008-2013 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Component.h"

struct lua_State;

namespace Urho3D
{

class LuaScript;

/// Lua Script object methods.
enum LuaScriptObjectMethod
{
    LSOM_START = 0,
    LSOM_STOP,
    // LSOM_DELAYEDSTART,
    LSOM_UPDATE,
    LSOM_POSTUPDATE,
    LSOM_FIXEDUPDATE,
    LSOM_FIXEDPOSTUPDATE,
    LSOM_LOAD,
    LSOM_SAVE,
    LSOM_READNETWORKUPDATE,
    LSOM_WRITENETWORKUPDATE,
    LSOM_APPLYATTRIBUTES,
    MAX_LUA_SCRIPT_OBJECT_METHODS
};

/// Lua script instance.
class URHO3D_API LuaScriptInstance : public Component
{
    OBJECT(LuaScriptInstance);

public:
    /// Construct.
    LuaScriptInstance(Context* context);
    /// Destruct.
    ~LuaScriptInstance();
    /// Register object factory.
    static void RegisterObject(Context* context);

    ///// Apply attribute changes that can not be applied immediately. Called after scene load or a network update.
    virtual void ApplyAttributes();

    /// Create script object.
    bool CreateObject(const String& scriptObjectType);

    /// Create script object.
    bool CreateObject(const String& scriptFileName, const String& scriptObjectType);

    /// Set script file name.
    void SetScriptFileName(const String& scriptFileName);

    /// Set script object type.
    void SetScriptObjectType(const String& scriptObjectType);

    /// Set script file serialization attribute by calling a script function.
    void SetScriptDataAttr(PODVector<unsigned char> data);
    
    /// Set script network serialization attribute by calling a script function.
    void SetScriptNetworkDataAttr(PODVector<unsigned char> data);

    /// Script subscribe to an event that can by send by any sender.
    void ScriptSubscribeToEvent(const String& eventName, const String& functionName);

    /// Script unsubscribe from an event.
    void ScriptUnsubscribeFromEvent(const String& eventName);

    /// Script unsubscribe from all events.
    void ScriptUnsubscribeFromAllEvents();

    /// Script subscribe to a specific sender's event.
    void ScriptSubscribeToEvent(void* sender, const String& eventName, const String& functionName);

    /// Script unsubscribe from a specific sender's event.
    void ScriptUnsubscribeFromEvent(void* sender, const String& eventName);

    /// Script unsubscribe from a specific sender's all events.
    void ScriptUnsubscribeFromEvents(void* sender);

    /// Return script file name.
    const String& GetScriptFileName() const { return scriptFileName_; }

    /// Return script object type.
    const String& GetScriptObjectType() const { return scriptObjectType_; }

    /// Return script object ref.
    int GetScriptObjectRef() const { return scriptObjectRef_; }

    /// Get script file serialization attribute by calling a script function.
    PODVector<unsigned char> GetScriptDataAttr() const;

    /// Get script network serialization attribute by calling a script function.
    PODVector<unsigned char> GetScriptNetworkDataAttr() const;

private:
    /// Find script object method refs.
    void FindScriptObjectMethodRefs();

    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    /// Handle the logic post update event.
    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);

    /// Handle the physics update event.
    void HandleFixedUpdate(StringHash eventType, VariantMap& eventData);

    /// Handle the physics post update event.
    void HandlePostFixedUpdate(StringHash eventType, VariantMap& eventData);

    /// Handle event.
    void HandleEvent(StringHash eventType, VariantMap& eventData);

    /// Handle object event.
    void HandleObjectEvent(StringHash eventType, VariantMap& eventData);

    /// Release the script object.
    void ReleaseObject();
    
    /// Call script object function.
    void CallScriptObjectFunction(int functionRef);

    /// Call script object function.
    void CallScriptObjectFunction(int functionRef, float timeStep);

    /// Call script object function.
    void CallScriptObjectFunction(int functionRef, Deserializer& deserializer);

    /// Call script object function.
    void CallScriptObjectFunction(int functionRef, Serializer& serializer) const;

    /// Call script object function.
    void CallScriptObjectFunction(int functionRef, StringHash eventType, VariantMap& eventData);

    // Lua Script.
    LuaScript* luaScript_;

    /// Lua state.
    lua_State* luaState_;

    /// Script file name.
    String scriptFileName_;

    /// Script object type.
    String scriptObjectType_;

    /// Script object ref.
    int scriptObjectRef_;

    /// Script object method refs.
    int scriptObjectMethodRefs_[MAX_LUA_SCRIPT_OBJECT_METHODS];

    /// Event type to function ref map.
    HashMap<StringHash, int> eventTypeToFunctionRefMap_;
    
    /// Object to event type to function ref map.
    HashMap<Object*, HashMap<StringHash, int> > objectToEventTypeToFunctionRefMap_;
};

}