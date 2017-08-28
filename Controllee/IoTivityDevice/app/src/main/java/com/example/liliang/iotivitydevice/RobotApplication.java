package com.example.liliang.iotivitydevice;

import android.app.Application;
import android.content.Context;

/**
 * Created by liliang on 17-8-23.
 */

public class RobotApplication extends Application {
    private Context context;

    @Override
    public void onCreate() {
        super.onCreate();
        context = this.getApplicationContext();
    }
}
