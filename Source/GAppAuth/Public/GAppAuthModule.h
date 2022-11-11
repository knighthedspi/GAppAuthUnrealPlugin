#pragma once

#include "Modules/ModuleManager.h"

class IGAppAuthModule : public IModuleInterface
{
public:
	static inline IGAppAuthModule &Get()
	{
		return FModuleManager::LoadModuleChecked<IGAppAuthModule>("GAppAuthModule");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GAppAuthModule");
	}
};
