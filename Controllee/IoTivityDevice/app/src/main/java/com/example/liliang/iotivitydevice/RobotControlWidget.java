package com.example.liliang.iotivitydevice;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;

/**
 * Created by liliang on 17-8-24.
 */

public class RobotControlWidget extends RelativeLayout implements View.OnClickListener, View.OnTouchListener {
    private final String TAG = RobotControlWidget.class.getName();
    private RelativeLayout btnForeBg;
    private Button leftBtn;
    private Button rightBtn;
    private Button topBtn;
    private Button downBtn;
    private Button centerBtn;

    private void initView(){
        LayoutInflater layoutInflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflater.inflate(R.layout.device_control_view, this, false);
        RelativeLayout.LayoutParams params = (LayoutParams) view.getLayoutParams();
        params.addRule(RelativeLayout.CENTER_IN_PARENT);

        btnForeBg = (RelativeLayout) view.findViewById(R.id.control_button_fore_bg);
        leftBtn = (Button) view.findViewById(R.id.left_command_button);
        leftBtn.setOnTouchListener(this);
        leftBtn.setOnClickListener(this);
        rightBtn = (Button) view.findViewById(R.id.right_command_button);
        rightBtn.setOnTouchListener(this);
        rightBtn.setOnClickListener(this);
        topBtn = (Button) view.findViewById(R.id.up_command_button);
        topBtn.setOnTouchListener(this);
        topBtn.setOnClickListener(this);
        downBtn = (Button) view.findViewById(R.id.down_command_button);
        downBtn.setOnTouchListener(this);
        downBtn.setOnClickListener(this);
        centerBtn = (Button) view.findViewById(R.id.center_command_button);
        addView(view);
    }

    public RobotControlWidget(Context context) {
        super(context);
        initView();
    }

    public RobotControlWidget(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public RobotControlWidget(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    public RobotControlWidget(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        initView();
    }

    @Override
    public void onClick(View view) {

    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        Log.d(TAG, "!!!!!!!!!!!!!!!!!!!!!!!!");
        if(view.getId() == R.id.left_command_button && motionEvent.getAction() == MotionEvent.ACTION_DOWN){
            btnForeBg.setForeground(getResources().getDrawable(R.drawable.rvc_control_btn_l_pressed_left, null));
            btnForeBg.setForegroundGravity(Gravity.LEFT);
        } else if (view.getId() == R.id.right_command_button && motionEvent.getAction() == MotionEvent.ACTION_DOWN){
            btnForeBg.setForeground(getResources().getDrawable(R.drawable.rvc_control_btn_l_pressed_right, null));
            btnForeBg.setForegroundGravity(Gravity.RIGHT);
        } else if (view.getId() == R.id.up_command_button && motionEvent.getAction() == MotionEvent.ACTION_DOWN){
            btnForeBg.setForeground(getResources().getDrawable(R.drawable.rvc_control_btn_l_pressed_up, null));
            btnForeBg.setForegroundGravity(Gravity.TOP);
        } else if (view.getId() == R.id.down_command_button && motionEvent.getAction() == MotionEvent.ACTION_DOWN){
            btnForeBg.setForeground(getResources().getDrawable(R.drawable.rvc_control_btn_l_pressed_down, null));
            btnForeBg.setForegroundGravity(Gravity.BOTTOM);
        } else if(motionEvent.getAction() != MotionEvent.ACTION_MOVE){
            btnForeBg.setForeground(null);
            //repeal the foreground effect
        }
        return false;
    }
}
