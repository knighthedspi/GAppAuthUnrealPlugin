
#include "GAppAuthModule.h"
#include "GAppAuthSettings.h"
#include "Core.h"
#include "Modules/ModuleManager.h"

#include "Misc/ConfigCacheIni.h"
#include "UObject/Package.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "GAppAuthModule"

class GAppAuthModule : public IGAppAuthModule
{
    virtual void StartupModule() override
    {
        // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
        Settings = NewObject<UGAppAuthSettings>(GetTransientPackage(), "GAppAuthSettings", RF_Standalone);
        Settings->AddToRoot();
        if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
        {
            SettingsModule->RegisterSettings("Project", "Plugins", "GAppAuth",
                                             LOCTEXT("GAppAuthSettingsName", "GAppAuth"),
                                             LOCTEXT("GAppAuthSettingsDescription", "Settings for Google App Client ID Prefix"),
                                             Settings);
        }
    }

    virtual void ShutdownModule() override
    {
        // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
        // we call this function before unloading the module.
        if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
        {
            SettingsModule->UnregisterSettings("Project", "Plugins", "GAppAuth");
        }

        if (!GExitPurge)
        {
            Settings->RemoveFromRoot();
        }
    }

private:
    UGAppAuthSettings *Settings;
};

IMPLEMENT_MODULE(GAppAuthModule, GAppAuth)

#undef LOCTEXT_NAMESPACE
