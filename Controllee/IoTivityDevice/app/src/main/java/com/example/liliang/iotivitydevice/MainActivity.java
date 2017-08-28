package com.example.liliang.iotivitydevice;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentTabHost;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import com.example.liliang.iotivitydevice.CloudConnector.LoginActivity;
import com.example.liliang.iotivitydevice.Device.DeviceManager;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = MainActivity.class.getName();
    public static final int LOGIN_ACTIVITY = 1;

    private FragmentTabHost fragmentTabHost;


    private String deviceId = "";
    private DeviceManager deviceManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getTheme().applyStyle(R.style.DeviceBaseTheme, true);

        LinearLayout toolbarLayout = (LinearLayout) findViewById(R.id.device_toolbar);
        Toolbar toolbar = new Toolbar(this);
        toolbar.setLayoutParams(new Toolbar.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT, 11));

        toolbar.setOverflowIcon(getDrawable(R.drawable.rvc_ic_title_more));
        toolbar.setTitle(getString(R.string.robot));
        toolbarLayout.addView(toolbar);

        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        fragmentTabHost = (FragmentTabHost) findViewById(R.id.tabhost);
        fragmentTabHost.setup(this, getSupportFragmentManager(), android.R.id.tabcontent);
        fragmentTabHost.addTab(fragmentTabHost.newTabSpec("ROBOT_CONTROLLER").setIndicator("CONTROLLER"), RobotControlFragment.class, null);
        fragmentTabHost.addTab(fragmentTabHost.newTabSpec("MAP_GO").setIndicator("MAP & GO"), MapFragment.class, null);

        deviceManager = new DeviceManager();
        deviceManager.Init(getApplicationContext());
        deviceId = deviceManager.createDeviceId();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_login) {
            Intent intent = new Intent(getApplicationContext(), LoginActivity.class);
            intent.putExtra("deviceId", deviceId);
            startActivityForResult(intent, LOGIN_ACTIVITY);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Log.d(TAG, "onActivityResult"+resultCode);
        switch (requestCode){
            case LOGIN_ACTIVITY:
                String authURL = data.getStringExtra("authServerUrl");
                String authCode = data.getStringExtra("authCode");
                String clientId = data.getStringExtra("client_id");
                deviceManager.requestAccessToken(authCode, authURL, clientId);
                Log.d(TAG, "authURL:"+authURL+",authcode:"+authCode);
                break;
            default:
                break;
        }
    }
}
