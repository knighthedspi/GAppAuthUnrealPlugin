#include "GAccountJniWrapper.h"
#include "GAppAuthModule.h"
#include "json98.h"
#include "GAccount.h"
#include "Async/AsyncWork.h"

using namespace std;
using namespace jsonParser;

DEFINE_LOG_CATEGORY(GAppAuth_Native);

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"

// Initialize JNI context
#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv *Env = FAndroidApplication::GetJavaEnv()) \
{ \
    name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
    check(name != NULL); \
} \
else \
{ \
    check(0); \
} \

#define DECLARE_JAVA_METHOD(name) \
static jmethodID name = NULL;

inline string jstring2string(JNIEnv *env, jstring jstr)
{
    if (jstr == nullptr || !env)
    {
        return "";
    }

    const char *chars = env->GetStringUTFChars(jstr, nullptr);
    string ret(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

inline FString jstring2FString(JNIEnv *env, jstring jstr)
{
    if (jstr == nullptr || !env)
    {
        return "";
    }

    const char *chars = env->GetStringUTFChars(jstr, nullptr);
    FString ret = FString(UTF8_TO_TCHAR(chars));
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

inline GAccountInfo convertFromJsonString(const string data)
{
    auto jsonData = Json::parse(data);
    return GAccountInfo(jsonData);
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnLoginSuccess(JNIEnv *env, jobject thiz, jstring data_)
{
    string data = jstring2string(env, data_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { gAppAuth::AccountJniWrapper::getInstance()->onLoginSuccess(data); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnLoginFailure(JNIEnv *env, jobject thiz, jstring message_)
{
    FString message = jstring2FString(env, message_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { gAppAuth::AccountJniWrapper::getInstance()->onLoginFailure(message); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnLogoutSuccess(JNIEnv *env, jobject thiz)
{
    AsyncTask(ENamedThreads::GameThread, [=]()
              { gAppAuth::AccountJniWrapper::getInstance()->onLogoutSuccess(); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnLogoutFailure(JNIEnv *env, jobject thiz, jstring message_)
{
    FString message = jstring2FString(env, message_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { gAppAuth::AccountJniWrapper::getInstance()->onLogoutFailure(message); });
}

namespace gAppAuth
{

    DECLARE_JAVA_METHOD(GAppAuth_Login);
    DECLARE_JAVA_METHOD(GAppAuth_Logout);
    
    static AccountJniWrapper *s_instance = nullptr;

    AccountJniWrapper *AccountJniWrapper::getInstance()
    {
        if (!s_instance)
        {
            s_instance = new AccountJniWrapper();
        }

        return s_instance;
    }

    AccountJniWrapper::AccountJniWrapper()
    {
        _listener = nullptr;

        INIT_JAVA_METHOD(GAppAuth_Login, "()V");
        INIT_JAVA_METHOD(GAppAuth_Logout, "()V");
    }

    AccountJniWrapper::~AccountJniWrapper()
    {
    }

    void AccountJniWrapper::login()
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
        {
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GAppAuth_Login);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method GAppAuth_Login\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

    void AccountJniWrapper::logout()
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
        {
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GAppAuth_Logout);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method GAppAuth_Logout\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

   
    /********************************************
     * Listener
     ********************************************/

    void AccountJniWrapper::setListener(AccountListener *listener)
    {
        _listener = listener;
    }

    void AccountJniWrapper::removeListener()
    {
        _listener = nullptr;
    }

    AccountListener *AccountJniWrapper::getListener()
    {
        return _listener;
    }

    /********************************************
     * Callbacks
     ********************************************/

    void AccountJniWrapper::onLoginSuccess(const string account)
    {
        UE_LOG(GAppAuth_Native, Log, TEXT("Log in success"));
        if (_listener != nullptr)
        {
            _listener->onLoginSuccess(convertFromJsonString(account));
        }
    }

    void AccountJniWrapper::onLoginFailure(const FString message)
    {
        UE_LOG(GAppAuth_Native, Log, TEXT("Login failure"));
        if (_listener != nullptr)
        {
            _listener->onLoginFailure(message);
        }
    }

    void AccountJniWrapper::onLogoutSuccess()
    {
        UE_LOG(GAppAuth_Native, Log, TEXT("Log out success"));
        if (_listener != nullptr)
        {
            _listener->onLogoutSuccess();
        }
    }

    void AccountJniWrapper::onLogoutFailure(const FString message)
    {
        UE_LOG(GAppAuth_Native, Log, TEXT("Log out failure"));
        if (_listener != nullptr)
        {
            _listener->onLogoutFailure(message);
        }
    }

}

#else

// Stub function for other platforms
namespace gAppAuth
{

    static AccountJniWrapper *s_instance = nullptr;

    AccountJniWrapper *AccountJniWrapper::getInstance()
    {
        if (!s_instance)
        {
            s_instance = new AccountJniWrapper();
        }

        return s_instance;
    }

    AccountJniWrapper::AccountJniWrapper()
    {
        _listener = nullptr;
    }

    AccountJniWrapper::~AccountJniWrapper()
    {
    }

    void AccountJniWrapper::login()
    {
        UE_LOG(GAppAuth_Native, Warning, TEXT("gAppAuth is not supported on this platform\n"));
    }

    void AccountJniWrapper::logout()
    {
        UE_LOG(GAppAuth_Native, Warning, TEXT("gAppAuth is not supported on this platform\n"));
    }

    /********************************************
     * Listener
     ********************************************/

    void AccountJniWrapper::setListener(AccountListener *listener)
    {
        _listener = listener;
    }

    void AccountJniWrapper::removeListener()
    {
        _listener = nullptr;
    }

    AccountListener *AccountJniWrapper::getListener()
    {
        return _listener;
    }

    /********************************************
     * Callbacks
     ********************************************/

    void AccountJniWrapper::onLoginSuccess(const string account)
    {
        UE_LOG(GAppAuth_Native, Warning, TEXT("gAppAuth is not supported on this platform\n"));
    }

    void AccountJniWrapper::onLoginFailure(const FString message)
    {
        UE_LOG(GAppAuth_Native, Warning, TEXT("gAppAuth is not supported on this platform\n"));
    }

    void AccountJniWrapper::onLogoutSuccess()
    {
        UE_LOG(GAppAuth_Native, Warning, TEXT("gAppAuth is not supported on this platform\n"));
    }

    void AccountJniWrapper::onLogoutFailure(const FString message)
    {
        UE_LOG(GAppAuth_Native, Warning, TEXT("gAppAuth is not supported on this platform\n"));
    }

}

#endif
