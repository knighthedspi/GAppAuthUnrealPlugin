#pragma once
#include "GAccount.h"
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(GAppAuth_Native, Log, All);
using namespace std;

namespace gAppAuth
{

    class AccountJniWrapper
    {

    public:
        static AccountJniWrapper *getInstance();

        AccountJniWrapper();
        ~AccountJniWrapper();

        void login();
        void logout();
        
        // Listener
        void setListener(AccountListener *listener);
        void removeListener();
        AccountListener *getListener();

        // Callbacks
        void onLoginSuccess(const string account);
        void onLoginFailure(const FString message);
        void onLogoutSuccess();
        void onLogoutFailure(const FString message);
        
    protected:
        AccountListener *_listener;
    };
}
