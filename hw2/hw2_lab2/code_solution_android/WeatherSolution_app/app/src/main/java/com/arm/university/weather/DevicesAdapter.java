package com.arm.university.weather;

import android.bluetooth.BluetoothDevice;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by tomcot01 on 21/08/2014.
 * ARM University Program Copyright © ARM Ltd 2014
 */
public class DevicesAdapter extends BaseAdapter {
    private ArrayList<BluetoothDevice> mDevices;
    private LayoutInflater mInflater;
    private boolean mScanning;

    public DevicesAdapter(LayoutInflater inflater) {
        super();
        mDevices = new ArrayList<BluetoothDevice>();
        mInflater = inflater;
    }

    public void updateScanningState(boolean isScanning) {
        mScanning = isScanning;
    }

    public void add(BluetoothDevice device) {
        if (!mDevices.contains(device))
            mDevices.add(device);
    }

    public void remove(int index) {
        mDevices.remove(index);
    }

    public void clear() {
        mDevices.clear();
    }

    @Override
    public int getCount() {
        return mDevices.size();
    }

    @Override
    public Object getItem(int i) {
        return mDevices.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        BluetoothDevice device = mDevices.get(i);

        if (view == null) {
            view = mInflater.inflate(R.layout.list_device, null);
        }

        TextView deviceNameTxt = (TextView) view.findViewById(R.id.device_name);
        TextView deviceAddressTxt = (TextView) view.findViewById(R.id.device_address);

        String deviceName;
        String deviceAddress;
        if (device != null) {
            deviceName = device.getName();
            deviceAddress = device.getAddress();
        } else {
            deviceName = " ";
            deviceAddress = mScanning ? "Scanning..." : "Found 0 devices";
        }
        if (deviceName != null && deviceName.length() > 0)
            deviceNameTxt.setText(deviceName);
        else
            deviceNameTxt.setText(R.string.unknown_device_name);

        deviceAddressTxt.setText(deviceAddress);

        return view;
    }
}
