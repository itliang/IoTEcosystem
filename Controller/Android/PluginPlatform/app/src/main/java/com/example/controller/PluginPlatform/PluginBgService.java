package com.example.controller.PluginPlatform;

import android.app.Notification;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.annotation.IntDef;
import android.support.annotation.Nullable;
import android.util.Log;


import java.util.ArrayList;
import java.util.List;

/**
 * Created by liliang on 17-3-23.
 */

public class PluginBgService extends Service {
    public final String TAG = this.getClass().getSimpleName();


    private IPluginBgService.Stub pluginBgService = new IPluginBgService.Stub(){

    };



    @Override
    public void onCreate() {
        Log.d(TAG, "AIDLService onCreate()");
        super.onCreate();

        //Notification notification = new Notification(null,"hello" );

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "AIDLService onStartCommand()");
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "AIDLService onDestroy()");
        super.onDestroy();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        Log.d(TAG, String.format("on bind, intent =%s", intent.toString()));

        return pluginBgService;
    }
}
