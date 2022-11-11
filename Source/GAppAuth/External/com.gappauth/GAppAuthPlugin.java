package com.gappauth;

import org.gappauth.sdk.api.GAppAuth;
import org.gappauth.sdk.entity.GSignInAccount;
import org.gappauth.sdk.entity.GSignInOptions;

import android.app.Activity;
import android.content.Intent;
import android.util.Log;

import androidx.annotation.NonNull;

public class GAppAuthPlugin {
    private static boolean isInit = false;
    private static Activity mActivity = null;
    private static GAppAuthListener mListener = null;
    private static GAppAuth gAppAuth = null;
    private static final int RC_AUTH = 100;
    private static final String TAG = "GAppAuthPlugin";

    public static void init(Activity activity, GAppAuthListener listener) {
        if (!isInit) {
            isInit = true;
            mActivity = activity;
            mListener = listener;
            GSignInOptions signInOptions = new GSignInOptions.Builder(GSignInOptions.DEFAULT_SIGN_IN)
                .requestEmail()
                .build();
            gAppAuth = new GAppAuth(mActivity, signInOptions);
        }
    }

    public static void login() {
        if (gAppAuth != null) {
            gAppAuth.signIn(RC_AUTH);
        }
    }

    public static void registerOnActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == RC_AUTH && gAppAuth != null) {
            gAppAuth.parseAuthResultFromIntent(data, new GAppAuth.OnSignInListener() {
                @Override
                public void onSuccess(@NonNull GSignInAccount account) {
                    Log.d(TAG, "Log in success with " + account + "\r\n");
                    if (mListener != null) {
                        mListener.onLoginSuccess(account.getJsonResult());
                    }
                }

                @Override
                public void onFailure(@NonNull Exception e) {
                    Log.e(TAG, "Log in failure", e);
                    if (mListener != null) {
                        mListener.onLoginFailure(e.getLocalizedMessage());
                    }
                }
            });
        }
    }

    public static void logout() {
        if (gAppAuth != null) {
            gAppAuth.signOut(new GAppAuth.OnSignOutListener() {
                @Override
                public void onSuccess() {
                    Log.d(TAG, "Logout success");
                    if (mListener != null) {
                        mListener.onLogoutSuccess();
                    }
                }
    
                @Override
                public void onFailure(@NonNull Exception e) {
                    Log.e(TAG, "Logout failure", e);
                    if (mListener != null) {
                        mListener.onLogoutFailure(e.getLocalizedMessage());
                    }
                }
            });
        }   
    }
}
