#include "Plugin.h"

#include <string>

#define UNUSED(x) (void) x;

#ifdef LUA_DEFINE
# undef LUA_DEFINE
#endif
#define LUA_DEFINE(name) Define(#name, [](lua_State *L) -> int

Lua::LuaArgs_t CallLuaFunction(lua_State *ScriptVM, const char *LuaFunctionName, Lua::LuaArgs_t *Arguments) {
    Lua::LuaArgs_t ReturnValues;
    int ArgCount = lua_gettop(ScriptVM);
    lua_getglobal(ScriptVM, LuaFunctionName);
    int argc = 0;
    if (Arguments) {
        for (auto const &e : *Arguments) {
            Lua::PushValueToLua(e, ScriptVM);
            argc++;
        }
    }
    int Status = lua_pcall(ScriptVM, argc, LUA_MULTRET, 0);
    ArgCount = lua_gettop(ScriptVM) - ArgCount;
    if (Status == LUA_OK) {
        Lua::ParseArguments(ScriptVM, ReturnValues);
        lua_pop(ScriptVM, ArgCount);
    }
    return ReturnValues;
}

Plugin::Plugin() {
    LUA_DEFINE(CallJavaAction) {
        std::string json;
        Lua::ParseArguments(L, json);
        jstring str = Plugin::Get()->env->NewStringUTF(json.c_str());
        UNUSED(str)
        Plugin::Get()->env->CallStaticVoidMethod(Plugin::Get()->adapterClass, Plugin::Get()->adapterWriteMethod, str);
        return 1;
    });
    LUA_DEFINE(CallJavaReady) {
        UNUSED(L)
        Plugin::Get()->env->CallStaticVoidMethod(Plugin::Get()->adapterClass, Plugin::Get()->adapterReadyMethod);
        return 1;
    });
}

Plugin::~Plugin() {

}

void Plugin::startPackage(lua_State *lua){
    luaState = lua;
    static std::string optionString = "-Djava.class.path=OnsetJava-JNI-1.0.jar";
    JavaVMInitArgs vm_args;
    auto* options = new JavaVMOption[1];
    options[0].optionString = const_cast<char *>(optionString.c_str());
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    adapterClass = env->FindClass("LuaAdapter");
    adapterWriteMethod = env->GetStaticMethodID(adapterClass, "lua_write_line", "(Ljava/lang/String;)V");
    adapterReadyMethod = env->GetStaticMethodID(adapterClass, "lua_ready", "()V");
    adapterReadMethod = env->GetStaticMethodID(adapterClass, "lua_read_line", "()Ljava.lang.String;");
}

void Plugin::tick(){
    if(luaState != nullptr){
        while (true){
            jobject line = env->CallStaticObjectMethod(adapterClass, adapterReadMethod); // THIS CURRENTLY PRODUCES A SEGFAULT FOR SOME REASON IDK
            if(line == nullptr){
                break;
            }
            Lua::LuaArgs_t args;
            args.push_back(new Lua::LuaValue("OnJavaAction"));
            const char *json = (*env).GetStringUTFChars((jstring) line, nullptr);
            args.push_back(new Lua::LuaValue(json));
            CallLuaFunction(luaState, "CallEvent", &args);
        }
    }
}

void Plugin::stopPackage(){
    jvm->DestroyJavaVM();
}