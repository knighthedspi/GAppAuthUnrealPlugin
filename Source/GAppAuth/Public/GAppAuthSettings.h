#pragma once

#include "GAppAuthSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class UGAppAuthSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/* Google Client Id Prefix */
	UPROPERTY(Config, EditAnywhere, Category = "GAppAuth")
	FString ClientIdPrefix;
};
