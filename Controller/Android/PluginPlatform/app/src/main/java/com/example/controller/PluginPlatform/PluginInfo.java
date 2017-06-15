package com.example.controller.PluginPlatform;

import android.os.Parcel;
import android.os.Parcelable;

import java.io.Serializable;

/**
 * Created by liliang on 17-5-31.
 */

public class PluginInfo implements Parcelable, Serializable{

    public PluginInfo(Parcel in) {

    }

    public static final Creator<PluginInfo> CREATOR = new Creator<PluginInfo>() {
        @Override
        public PluginInfo createFromParcel(Parcel in) {
            return new PluginInfo(in);
        }

        @Override
        public PluginInfo[] newArray(int size) {
            return new PluginInfo[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {

    }
}
