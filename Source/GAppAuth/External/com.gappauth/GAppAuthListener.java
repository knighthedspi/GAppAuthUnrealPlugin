package com.gappauth;

public interface GAppAuthListener {
    void onLoginSuccess(String jsonResult);
    void onLoginFailure(String message);
    void onLogoutSuccess();
    void onLogoutFailure(String message);
}
