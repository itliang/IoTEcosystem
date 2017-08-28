package com.example.liliang.iotivitydevice;


import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListAdapter;
import android.widget.ListView;


public class RobotControlFragment extends Fragment {
    private RobotListAdapter listAdapter;
    private ListView listView;


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_device_control, container, false);
        listAdapter = new RobotListAdapter(getContext());
        listView = (ListView) rootView.findViewById(R.id.list_view);

        listView.setAdapter(listAdapter);
        listView.setItemsCanFocus(true);
        listView.setFocusable(false);
        listView.setFocusableInTouchMode(false);
        listView.setClickable(false);
        //listView.setOnItemClickListener(mMenuListClickListener);
        listView.setBackgroundColor(0xffebebeb);

        listAdapter.addItem(getResources().getDrawable(R.drawable.main_rv_no_history, null), getString(R.string.app_name),
                null, RobotListAdapter.SubItemType.NONE);
        listAdapter.addItem(getResources().getDrawable(R.drawable.main_rv_no_history, null), getString(R.string.app_name),
                getString(R.string.app_name), RobotListAdapter.SubItemType.INDICATOR);
        listAdapter.addItem(getResources().getDrawable(R.drawable.main_rv_no_history, null), getString(R.string.app_name),
                getString(R.string.app_name), RobotListAdapter.SubItemType.SWITCH);
        setListViewHeightBasedOnItems(listView);
        listAdapter.notifyDataSetChanged();
        setHasOptionsMenu(true);
        return rootView;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        // Inflate the menu; this adds items to the action bar if it is present.
        super.onCreateOptionsMenu(menu, inflater);
        inflater.inflate(R.menu.menu_main, menu);
        menu.findItem(R.id.menu_help).setVisible(true);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if(item.getItemId() == R.id.menu_help){

        }
        return super.onOptionsItemSelected(item);
    }

    public boolean setListViewHeightBasedOnItems(ListView listView) {
        ListAdapter listAdapter = listView.getAdapter();
        if (listAdapter != null) {
            int count = listAdapter.getCount();
            int totalHeight = 0;
            for (int itemPos = 0; itemPos < count; itemPos++) {
                View item = listAdapter.getView(itemPos, null, listView);
                item.measure(0, 0);
                totalHeight += item.getMeasuredHeight();
                totalHeight += listView.getDividerHeight();
            }
            ViewGroup.LayoutParams params = listView.getLayoutParams();
            params.height = totalHeight;
            listView.setLayoutParams(params);
            listView.requestLayout();

            return true;
        } else {
            return false;
        }
    }
}
