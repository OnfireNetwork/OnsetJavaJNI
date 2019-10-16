#pragma once

#include <PluginSDK.h>

#include "Singleton.h"

#include <vector>
#include <tuple>
#include <functional>
#include <jni.h>


class Plugin : public Singleton<Plugin>
{
    friend class Singleton<Plugin>;
private:
    JavaVM *jvm;
    JNIEnv *env;
    jclass adapterClass;
    jmethodID adapterWriteMethod;
    jmethodID adapterReadMethod;
    jmethodID adapterReadyMethod;
    bool attached = false;
    Plugin();
    ~Plugin();
public:
    lua_State *luaState;

private:
    using FuncInfo_t = std::tuple<const char *, lua_CFunction>;
    std::vector<FuncInfo_t> _func_list;

private:
    inline void Define(const char * name, lua_CFunction func)
    {
        _func_list.emplace_back(name, func);
    }

public:
    decltype(_func_list) const &GetFunctions() const
    {
        return _func_list;
    }

public:
    void startPackage(lua_State *lua);
    void stopPackage();
    void tick();

};