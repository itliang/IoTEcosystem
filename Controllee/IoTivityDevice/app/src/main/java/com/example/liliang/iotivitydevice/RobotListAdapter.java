package com.example.liliang.iotivitydevice;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by liliang on 17-8-24.
 */

public class RobotListAdapter extends BaseAdapter {
    private Context context;
    private ArrayList<ListItemData> arrayList;

    public class ViewHolder {
        public ImageView mIcon = null;
        public TextView mTitle = null;
        public TextView mSubTitle = null;
        public ImageView mIndicator = null;
        public Switch mSwitch = null;
    }

    public enum SubItemType {
        NONE,
        INDICATOR,
        SWITCH,
    }

    public class ListItemData{
        public Drawable mDrawableIcon = null;
        public String mMainTitleString = null;
        public String mSubTitleString = null;
        public SubItemType mSubItemType = SubItemType.NONE;
        public Boolean isEnable = true;
        public Boolean isChecked = false;
    }

    public RobotListAdapter(Context context) {
        this.context = context;
        arrayList = new ArrayList<ListItemData>();
    }

    public void addItem(Drawable icon, String title, String subTitle, RobotListAdapter.SubItemType type) {
        ListItemData data = new ListItemData();
        data.mDrawableIcon = icon;
        data.mMainTitleString = title;
        data.mSubTitleString = subTitle;
        data.mSubItemType = type;
        arrayList.add(data);
    }

    @Override
    public int getCount() {
        return arrayList.size();
    }

    @Override
    public ListItemData getItem(int i) {
        return arrayList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        ViewHolder holder = null;
        if (view == null) {
            holder = new ViewHolder();
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflater.inflate(R.layout.list_device_adapter, viewGroup, false);

            holder.mIcon = (ImageView) view.findViewById(R.id.rvc_item_icon);
            holder.mTitle = (TextView) view.findViewById(R.id.list_item_title);
            holder.mSubTitle = (TextView) view.findViewById(R.id.list_item_subtitle);
            holder.mIndicator = (ImageView) view.findViewById(R.id.list_item_indicator);
            holder.mSwitch = (Switch) view.findViewById(R.id.list_item_switch);
            view.setTag(holder);
        } else {
            holder = (ViewHolder) view.getTag();
        }

        ListItemData data = arrayList.get(i);
        if (data == null) {
            return view;
        }

        view.setClickable(!data.isEnable);
        if (data.mDrawableIcon == null) {
            holder.mIcon.setVisibility(View.GONE);
            holder.mIcon.setImageDrawable(null);
            holder.mIcon.setEnabled(data.isEnable);
        } else {
            holder.mIcon.setVisibility(View.VISIBLE);
            holder.mIcon.setImageDrawable(data.mDrawableIcon);
            holder.mIcon.setEnabled(data.isEnable);
        }

        if (data.mSubItemType == RobotListAdapter.SubItemType.NONE) {
            holder.mIndicator.setVisibility(View.GONE);
            holder.mSwitch.setVisibility(View.GONE);
        } else if (data.mSubItemType == RobotListAdapter.SubItemType.INDICATOR) {
            holder.mIndicator.setVisibility(View.VISIBLE);
            holder.mSwitch.setVisibility(View.GONE);
        }  else if (data.mSubItemType == RobotListAdapter.SubItemType.SWITCH) {
            holder.mIndicator.setVisibility(View.GONE);
            holder.mSwitch.setVisibility(View.VISIBLE);
        }
        holder.mIndicator.setEnabled(data.isEnable);
        holder.mSwitch.setEnabled(data.isEnable);

        if (data.mMainTitleString == null) {
            holder.mTitle.setText(null);
            holder.mTitle.setEnabled(data.isEnable);
        } else {
            holder.mTitle.setText(data.mMainTitleString);
            holder.mTitle.setEnabled(data.isEnable);
        }

        if (data.mSubTitleString == null) {
            holder.mSubTitle.setText(null);
            holder.mSubTitle.setVisibility(View.GONE);
            holder.mSubTitle.setEnabled(data.isEnable);
        } else {
            holder.mSubTitle.setText(data.mSubTitleString);
            holder.mSubTitle.setVisibility(View.VISIBLE);
            holder.mSubTitle.setEnabled(data.isEnable);
        }

        return view;
    }
}
