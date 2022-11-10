#include "GAccountBlueprint.h"

using namespace gAppAuth;

FGAccountInfo convertFromRawData(GAccountInfo info)
{
    FGAccountInfo fInfo;
    fInfo.id = info.id;
    fInfo.email = info.email;
    fInfo.displayName = info.displayName;
    fInfo.givenName = info.givenName;
    fInfo.photoUrl = info.photoUrl;
	 return fInfo;
}

FOnLogInSuccess UGAppAuthBlueprint::_onLogInSuccess;
FOnLogInFailure UGAppAuthBlueprint::_onLogInFailure;
void UGAppAuthBlueprint::login(const FOnLogInSuccess &onSuccess, const FOnLogInFailure &onFailure)
{
    Account::login();
    UGAppAuthBlueprint::_onLogInSuccess = onSuccess;
    UGAppAuthBlueprint::_onLogInFailure = onFailure;
}

FOnLogOutSuccess UGAppAuthBlueprint::_onLogOutSuccess;
FOnLogOutFailure UGAppAuthBlueprint::_onLogOutFailure;
void UGAppAuthBlueprint::logout(const FOnLogOutSuccess &onSuccess, const FOnLogOutFailure &onFailure)
{
    Account::logout();
    UGAppAuthBlueprint::_onLogOutSuccess = onSuccess;
    UGAppAuthBlueprint::_onLogOutFailure = onFailure;
}

void UGAppAuthBlueprint::onLoginSuccess(const GAccountInfo account)
{
	UGAppAuthBlueprint::_onLogInSuccess.ExecuteIfBound(convertFromRawData(account));
}

void UGAppAuthBlueprint::onLoginFailure(const FString message)
{
	UGAppAuthBlueprint::_onLogInFailure.ExecuteIfBound(message);
}

void UGAppAuthBlueprint::onLogoutSuccess()
{
	UGAppAuthBlueprint::_onLogOutSuccess.ExecuteIfBound();
}

void UGAppAuthBlueprint::onLogoutFailure(const FString message)
{
	UGAppAuthBlueprint::_onLogOutFailure.ExecuteIfBound(message);
}
