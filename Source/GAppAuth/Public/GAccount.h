#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include <string>
#include "json98.h"

namespace gAppAuth
{
    struct GAccountInfo
    {
        GAccountInfo(const jsonParser::Json &json);

        FString id;
        FString email;
        FString displayName;
        FString givenName;
        FString photoUrl;
    };

    class AccountListener
    {
    public:    
        virtual void onLoginSuccess(const GAccountInfo account) = 0;
        virtual void onLoginFailure(const FString message) = 0;
        virtual void onLogoutSuccess() = 0;
        virtual void onLogoutFailure(const FString message) = 0;
    };

    class Account
    {
    public:
        static void login();
        static void logout();
       
        static void setListener(AccountListener *listener);
        static void removeListener();
        static AccountListener *getListener();
    };
}