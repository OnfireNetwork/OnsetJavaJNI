#include <PluginSDK.h>
#include "Plugin.h"

Onset::IServerPlugin* Onset::Plugin::_instance = nullptr;

EXPORT(int) OnPluginGetApiVersion()
{
    return 1;
}

EXPORT(void) OnPluginCreateInterface(Onset::IBaseInterface *PluginInterface)
{
    Onset::Plugin::Init(PluginInterface);
}

EXPORT(int) OnPluginStart()
{
    Plugin::Get();
    return PLUGIN_API_VERSION;
}

EXPORT(void) OnPluginStop()
{
    Plugin::Singleton::Destroy();
    Onset::Plugin::Destroy();
}

EXPORT(void) OnPluginTick(float DeltaSeconds)
{
    (void) DeltaSeconds;
    Plugin::Get()->tick();
}

EXPORT(void) OnPackageLoad(const char *PackageName, lua_State *L)
{
    auto pn = new std::string(PackageName);
    if(*pn == "java"){
        for (auto const &f : Plugin::Get()->GetFunctions())
            Lua::RegisterPluginFunction(L, std::get<0>(f), std::get<1>(f));
        Plugin::Get()->startPackage(L);
    }
}

EXPORT(void) OnPackageUnload(const char *PackageName)
{
    auto pn = new std::string(PackageName);
    if(*pn == "java"){
        Plugin::Get()->stopPackage();
    }
}