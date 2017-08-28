package com.example.liliang.iotivitydevice.Device;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import android.content.Context;
import android.util.Log;

import com.example.liliang.iotivitydevice.CloudConnector.TokenProvider;

import java.nio.ByteBuffer;
import java.util.UUID;

/**
 * Created by liliang on 17-8-17.
 */

public class DeviceManager {
    public static final String TAG = DeviceManager.class.getName();

    public String authCode;
    public String authURL;
    public String deviceId;

    public void Init(Context context){

        PlatformConfig platformConfig = new PlatformConfig(
                context,
                ServiceType.IN_PROC,
                ModeType.CLIENT_SERVER,
                "0.0.0.0",
                0, // Uses randomly available port
                QualityOfService.LOW,
                ""
        );

        OcPlatform.Configure(platformConfig);


    }

    public void requestAccessToken(String authCode, String authURL, String clientId){
        TokenProvider tokenProvider = new TokenProvider(authURL, accessTokenListener);
        tokenProvider.getAccessToken(authURL, authCode, clientId, deviceId);
    }

    public String createDeviceId() {

        byte[] deviceIdBytes = OcPlatform.getDeviceId();
        Log.d(TAG, "!!!!!!!!!!!!!bb:"+deviceIdBytes.toString());
        ByteBuffer bb = ByteBuffer.wrap(deviceIdBytes);
        long high = bb.getLong();
        long low = bb.getLong();
        UUID uuid = new UUID(high, low);
        deviceId = uuid.toString();
        return deviceId;
    }

    TokenProvider.AccessTokenListener accessTokenListener = new TokenProvider.AccessTokenListener() {
        @Override
        public void onAccessTokenReceived(String accessToken, String refreshToken, String userId, String authProviderURL) {
            Log.d(TAG, "accessToken:"+accessToken+",refreshToken:"+refreshToken);
        }

        @Override
        public void onAccessTokenFailed() {

        }
    };
}
