#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "GAccount.h"
#include "Delegates/DelegateCombinations.h"
#include "GAccountBlueprint.generated.h"

using namespace gAppAuth;

USTRUCT(Blueprintable, BlueprintType)
struct GAPPAUTH_API FGAccountInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAppAuth")
    FString id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAppAuth")
    FString email;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAppAuth")
    FString displayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAppAuth")
    FString givenName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAppAuth")
    FString photoUrl;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLogInSuccess, FGAccountInfo, account);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLogInFailure, FString, message);
DECLARE_DYNAMIC_DELEGATE(FOnLogOutSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLogOutFailure, FString, message);

UCLASS()
class GAPPAUTH_API UGAppAuthBlueprint : public UBlueprintFunctionLibrary, public AccountListener
{
    GENERATED_BODY()

    UGAppAuthBlueprint(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
    {
        gAppAuth::Account::setListener(this);
    };

    static FOnLogInSuccess _onLogInSuccess;
    static FOnLogOutSuccess _onLogOutSuccess;
    static FOnLogInFailure _onLogInFailure;
    static FOnLogOutFailure _onLogOutFailure;
   
    UFUNCTION(BlueprintCallable, Category = "GAppAuth Category")
    static void login(const FOnLogInSuccess &onSuccess, const FOnLogInFailure &onFailure);

    UFUNCTION(BlueprintCallable, Category = "GAppAuth Category")
    static void logout(const FOnLogOutSuccess &onSuccess, const FOnLogOutFailure &onFailure);

    virtual void onLoginSuccess(const gAppAuth::GAccountInfo account) override;
    virtual void onLoginFailure(const FString message);
	virtual void onLogoutSuccess() override;
    virtual void onLogoutFailure(const FString message) override;
};
