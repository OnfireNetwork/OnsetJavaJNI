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
    Onset::IServerPlugin::
}

EXPORT(int) OnPluginStart()
{
    // initialize plugin singleton
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
    (void)DeltaSeconds; // unused
}

EXPORT(void) OnPackageLoad(const char *PackageName, lua_State *L)
{
    (void)PackageName; // unused
    (void) L;
}

EXPORT(void) OnPackageUnload(const char *PackageName)
{
    (void)PackageName; // unused
}