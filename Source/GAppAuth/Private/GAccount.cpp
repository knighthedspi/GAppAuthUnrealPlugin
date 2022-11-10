#include "GAccount.h"
#include "GAppAuthModule.h"
#include "GAccountJniWrapper.h"

using namespace std;
using namespace jsonParser;

namespace gAppAuth
{

    void Account::login()
    {
        AccountJniWrapper::getInstance()->login();
    }

    void Account::logout()
    {
        AccountJniWrapper::getInstance()->logout();
    }

    void Account::setListener(AccountListener *listener)
    {
        AccountJniWrapper::getInstance()->setListener(listener);
    }

    void Account::removeListener()
    {
        AccountJniWrapper::getInstance()->removeListener();
    }

    AccountListener *Account::getListener()
    {
        return AccountJniWrapper::getInstance()->getListener();
    }

    /********************************************
     * Data classes
     *******************************************/
    GAccountInfo::GAccountInfo(const jsonParser::Json &json)
    {
        id = FString(UTF8_TO_TCHAR(json["sub"].string_value().c_str()));
        email = FString(UTF8_TO_TCHAR(json["email"].string_value().c_str()));
        displayName = FString(UTF8_TO_TCHAR(json["name"].string_value().c_str()));
        givenName = FString(UTF8_TO_TCHAR(json["given_name"].string_value().c_str()));
        photoUrl = FString(UTF8_TO_TCHAR(json["picture"].string_value().c_str()));
    }
}