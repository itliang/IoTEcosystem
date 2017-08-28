package com.example.liliang.iotivitydevice.CloudConnector;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.UrlQuerySanitizer;
import android.os.Bundle;
import android.util.Log;
import android.webkit.WebResourceError;
import android.webkit.WebResourceRequest;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.example.liliang.iotivitydevice.R;

import org.iotivity.base.OcPlatform;

import java.nio.ByteBuffer;
import java.util.UUID;

public class LoginActivity extends Activity {
    private static final String TAG = LoginActivity.class.getName();
    public static final String SAMSUNG_ACCOUNT_URL = "https://account.samsung.com/mobile/account/check.do?serviceID=6iado3s6jc&actionID=StartOAuth2&countryCode=kr&languageCode=ko";
    public static final String KEY_SCOPE = "scope";
    public static final String SCOPE_DEVICE = "iot.device";
    public static final String KEY_DEVICEID = "deviceUniqueID";
    public static final String KEY_ADDRESS = "devicePhysicalAddressText";
    public static final String SAMSUNG_ACCOUNT_CLOSE_URL = "https://account.samsung.com/mobile/account/signInSuccessOAuth2.do";

    private WebView mWebView = null;

    private String loginURL;
    private String authCode;
    private String clientID;
    private String authServerUrl;
    private String apiServerUrl;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        String deviceId = intent.getStringExtra("deviceId");
        Log.d(TAG, "!!!!!!!!!!!!! deviceId : "+deviceId);

        setContentView(R.layout.activity_account_login);
        mWebView = (WebView)findViewById(R.id.webview);
        mWebView.setInitialScale(200);
        mWebView.getSettings().setJavaScriptEnabled(true);
        mWebView.getSettings().setBuiltInZoomControls(true);
        mWebView.clearCache(true);
        mWebView.setWebViewClient(new LoginWebViewClient());

        loginURL = SAMSUNG_ACCOUNT_URL + "&" + KEY_SCOPE + "=" + SCOPE_DEVICE
                + "&" + KEY_DEVICEID + "=" + deviceId + "&" + KEY_ADDRESS + "=" + deviceId;
        Log.d(TAG, "loginURL:"+loginURL);

        mWebView.loadUrl(loginURL);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private class LoginWebViewClient extends WebViewClient{
        @Override
        public void onPageStarted(WebView view, String url, Bitmap favicon) {
            super.onPageStarted(view, url, favicon);
            Log.d(TAG, "onPageStarted:"+url);
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            super.onPageFinished(view, url);
            Log.d(TAG, "onPageFinished:"+url);
            if (url.contains(SAMSUNG_ACCOUNT_CLOSE_URL)){
                UrlQuerySanitizer sanitizer = new UrlQuerySanitizer();
                sanitizer.setAllowUnregisteredParamaters(true);
                sanitizer.parseUrl(url);
                authCode = sanitizer.getValue("code");
                authServerUrl = sanitizer.getValue("auth_server_url");
                apiServerUrl = sanitizer.getValue("api_server_url");
                clientID = sanitizer.getValue("client_id");

                String httpsAuthServerUrl = "https://"+authServerUrl;
                String httpsApiServerUrl = "https://"+apiServerUrl;

                Intent intent = getIntent();
                intent.putExtra("authCode", authCode);
                intent.putExtra("authServerUrl", httpsAuthServerUrl);
                intent.putExtra("apiServerUrl", httpsApiServerUrl);
                intent.putExtra("client_id", clientID);

                setResult(RESULT_OK, intent);
                finish();
            }
        }

        @Override
        public void onLoadResource(WebView view, String url) {
            super.onLoadResource(view, url);
        }

        @Override
        public void onReceivedError(WebView view, WebResourceRequest request, WebResourceError error) {
            super.onReceivedError(view, request, error);
            Log.e(TAG, "onReceivedError:"+error.getErrorCode());
        }
    }

}
