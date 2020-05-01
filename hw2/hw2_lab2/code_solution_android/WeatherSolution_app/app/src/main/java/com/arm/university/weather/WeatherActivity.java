package com.arm.university.weather;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
 * Created by joatei01 on 13/05/2015.
                * ARM University Program Copyright © ARM Ltd 2015
                */
        public class WeatherActivity extends Activity {

            private static final String TAG = "WeatherActivity";

            AlertDialog mSelectionDialog;
            DevicesAdapter mDevicesAdapter;
            BluetoothAdapter mBluetoothAdapter;
            Handler mHandler;
            boolean mScanning;
            BluetoothGatt mGatt;
            private Queue<BluetoothGattDescriptor> descriptorWriteQueue = new LinkedList<>();
            private static final int SCAN_PERIOD = 10000;
            private static final int REQUEST_ENABLE_BT = 1;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mHandler = new Handler();
        mDevicesAdapter = new DevicesAdapter(getLayoutInflater());

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle(R.string.select_device);
        builder.setAdapter(mDevicesAdapter, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                finishScanning();
                BluetoothDevice device = (BluetoothDevice) mDevicesAdapter.getItem(i);
                if (device != null) {
                    Log.i(TAG, "Connecting to GATT server at: " + device.getAddress());
                    mGatt = device.connectGatt(WeatherActivity.this, false, mGattCallback);
                }
            }
        });
        builder.setNegativeButton(R.string.cancel, null);
        builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialogInterface) {
                finishScanning();
            }
        });
        mSelectionDialog = builder.create();

        BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();

        setContentView(R.layout.activity_weather);
    }

    public void onConnectClick(View view) {
       if (mBluetoothAdapter == null) {
           Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
           startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
         } else {
            String btnText = ((Button) view).getText().toString();
            if (btnText.equals(getString(R.string.connect))) {
                openSelectionDialog();
            } else if (btnText.equals(getString(R.string.disconnect))) {
                mGatt.disconnect();
                mGatt.close();
                updateConnectButton(BluetoothProfile.STATE_DISCONNECTED);
            }
        }
    }

    void openSelectionDialog() {
        beginScanning();
        mSelectionDialog.show();
    }

    private void beginScanning() {
        if (!mScanning) {
            // Stops scanning after a pre-defined scan period.
            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    finishScanning();
                }
            }, SCAN_PERIOD);

            mDevicesAdapter.clear();
            mDevicesAdapter.add(null);
            mDevicesAdapter.updateScanningState(mScanning = true);
            mDevicesAdapter.notifyDataSetChanged();
            mBluetoothAdapter.startLeScan(mScanCallback);
        }
    }

    private void finishScanning() {
        if (mScanning) {
            if (mDevicesAdapter.getItem(0) == null) {
                mDevicesAdapter.notifyDataSetChanged();
            }
            mBluetoothAdapter.stopLeScan(mScanCallback);
        }
    }

    private void disconnect() {
        if (mGatt != null) {
            mGatt.disconnect();
            mGatt.close();
            mGatt = null;
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        finishScanning();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        disconnect();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_BT) {
            if (resultCode == Activity.RESULT_OK) {
                openSelectionDialog();
            } else if (resultCode == Activity.RESULT_CANCELED) {
                Toast.makeText(this, "App cannot run with bluetooth off", Toast.LENGTH_LONG).show();
                finish();
            }
        } else
            super.onActivityResult(requestCode, resultCode, data);
    }

    private void updateConnectButton(int state) {
        Button connectBtn = (Button) WeatherActivity.this.findViewById(R.id.connect_button);
        switch (state) {
            case BluetoothProfile.STATE_DISCONNECTED:
                connectBtn.setText(getString(R.string.connect));
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        TextView humidityTxt = (TextView) WeatherActivity.this.findViewById(R.id.Hvalue);
                        humidityTxt.setText(R.string.Hvalue);
                        TextView temperatureTxt = (TextView) WeatherActivity.this.findViewById(R.id.Tvalue);
                        temperatureTxt.setText(R.string.Tvalue);
                        TextView windTxt = (TextView) WeatherActivity.this.findViewById(R.id.Wvalue);
                        windTxt.setText(R.string.Wvalue);
                        TextView pressureTxt = (TextView) WeatherActivity.this.findViewById(R.id.Pvalue);
                        pressureTxt.setText(R.string.Pvalue);
                    }
                });
                break;
            case BluetoothProfile.STATE_CONNECTING:
                connectBtn.setText(getString(R.string.connecting));
                break;
            case BluetoothProfile.STATE_CONNECTED:
                connectBtn.setText(getString(R.string.disconnect));
                break;
        }
    }

    private BluetoothAdapter.LeScanCallback mScanCallback = new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice bluetoothDevice, int i, final byte[] bytes) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {

                    if (mDevicesAdapter.getItem(0) == null) {
                        mDevicesAdapter.remove(0);
                    }
                    mDevicesAdapter.add(bluetoothDevice);
                    mDevicesAdapter.notifyDataSetChanged();
                    //}
                }
            });
        }
    };

    private BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, final int newState) {
            super.onConnectionStateChange(gatt, status, newState);

            if (newState == BluetoothProfile.STATE_CONNECTED) {
                Log.i(TAG, "Connected to GATT server.");
                if (mGatt.discoverServices()) {
                    Log.i(TAG, "Started service discovery.");
                } else {
                    Log.w(TAG, "Service discovery failed.");
                }
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                Log.i(TAG, "Disconnected from GATT server.");
            }
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    updateConnectButton(newState);
                }
            });
        }

        /******************* Callback called when a service is discovered *************************/
        /****** Make a list of supported GATT services and subscribe to the characteristics. ******/

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            super.onServicesDiscovered(gatt, status);

            List<BluetoothGattService> gattServices = getSupportedGattServices();

            if (gattServices == null) {
                Log.w(TAG, "Not services found.");
                return;
            }

            for (BluetoothGattService gattService : gattServices) {
                List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();
                for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {

                    if (gattCharacteristic.getUuid().equals(AssignedNumber.getBleUuid("Humidity"))) {

                        mGatt.setCharacteristicNotification(gattCharacteristic, true);// Enable notifications.

                        BluetoothGattDescriptor descriptor = gattCharacteristic.getDescriptor(AssignedNumber.getBleUuid("Client Characteristic Configuration"));

                        descriptor.setValue( BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE );

                        writeGattDescriptor(descriptor);
                        Log.i(TAG, "Humidity characteristic subscription done");
                    }

                    if (gattCharacteristic.getUuid().equals(AssignedNumber.getBleUuid("Temperature"))) {

                        mGatt.setCharacteristicNotification(gattCharacteristic, true);// Enable notifications.

                        BluetoothGattDescriptor descriptor = gattCharacteristic.getDescriptor(AssignedNumber.getBleUuid("Client Characteristic Configuration"));

                        descriptor.setValue( BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE );

                        writeGattDescriptor(descriptor);
                        Log.i(TAG, "Temperature characteristic subscription done");
                    }

                    if (gattCharacteristic.getUuid().equals(AssignedNumber.getBleUuid("True Wind Direction"))) {

                        mGatt.setCharacteristicNotification(gattCharacteristic, true);// Enable notifications.

                        BluetoothGattDescriptor descriptor = gattCharacteristic.getDescriptor(AssignedNumber.getBleUuid("Client Characteristic Configuration"));

                        descriptor.setValue( BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE );

                        writeGattDescriptor(descriptor);
                        Log.i(TAG, "Wind direction characteristic subscription done");
                    }

                    if (gattCharacteristic.getUuid().equals(AssignedNumber.getBleUuid("Pressure"))) {

                        mGatt.setCharacteristicNotification(gattCharacteristic, true);// Enable notifications.

                        BluetoothGattDescriptor descriptor = gattCharacteristic.getDescriptor(AssignedNumber.getBleUuid("Client Characteristic Configuration"));

                        descriptor.setValue( BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE );

                        writeGattDescriptor(descriptor);
                        Log.i(TAG, "Pressure characteristic subscription done");
                    }
                }
            }
        }


        /******************** Callback called when a characteristic change ************************/
        /****** Check which characteristic has changed and update the TextView accordingly ********/

        @Override
        public void onCharacteristicChanged (BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicChanged(gatt, characteristic);

            //Log.i(TAG, "One characteristic has changed");

            if (characteristic.getUuid().equals(AssignedNumber.getBleUuid("Humidity"))) {
                Log.i(TAG, "Humidity has changed");

                float humidity100 = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT16,0).floatValue();
                final float humidity = humidity100 / 100.0f;  // 2 decimals
                runOnUiThread(new Runnable() {
                    public void run() {
                        TextView humidityTxt = (TextView) WeatherActivity.this.findViewById(R.id.Hvalue);
                        humidityTxt.setText(String.format("%.2f%%", humidity));
                    }
                 });
            Log.d(TAG, String.format("Update humidity: %.2f%%", humidity));
            }

            if (characteristic.getUuid().equals(AssignedNumber.getBleUuid("Temperature"))) {

                Log.i(TAG, "Temperature has changed");

                float temperature100 = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_SINT16,0).floatValue();
                final float temperature = temperature100 / 100.0f; // 2 decimals
                runOnUiThread(new Runnable() {
                    public void run() {
                        TextView temperatureTxt = (TextView) WeatherActivity.this.findViewById(R.id.Tvalue);
                        temperatureTxt.setText(String.format("%.2f°C", temperature));
                    }
                });
                Log.d(TAG, String.format("Update temperature: %.2f°C", temperature));
            }

            if (characteristic.getUuid().equals(AssignedNumber.getBleUuid("True Wind Direction"))) {

                Log.i(TAG, "Wind direction has changed");

                int wind100 = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT16,0);
                final int wind = wind100/ 100;
                runOnUiThread(new Runnable() {
                    public void run() {
                        TextView windTxt = (TextView) WeatherActivity.this.findViewById(R.id.Wvalue);
                        if ((wind >= 0 && wind <= 22)||(wind >= 338 && wind <= 360 ) )  windTxt.setText("N");
                        if (wind >= 23 && wind <= 67)  windTxt.setText("NE");
                        if (wind >= 68 && wind <= 112)  windTxt.setText("E");
                        if (wind >= 113 && wind <= 157)  windTxt.setText("SE");
                        if (wind >= 158 && wind <= 202)  windTxt.setText("S");
                        if (wind >= 203 && wind <= 247)  windTxt.setText("SW");
                        if (wind >= 248 && wind <= 292)  windTxt.setText("W");
                        if (wind >= 293 && wind <= 337)  windTxt.setText("NW");
                    }
                });
                Log.d(TAG, String.format("Update wind: %d", wind));
            }

            if (characteristic.getUuid().equals(AssignedNumber.getBleUuid("Pressure"))) {

                Log.i(TAG, "Pressure has changed");

                float pressure1000 = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT32,0);
                final float pressure = pressure1000 / 1000.0f; //Convert to hPa with one decimal
                runOnUiThread(new Runnable() {
                    public void run() {
                        TextView pressureTxt = (TextView) WeatherActivity.this.findViewById(R.id.Pvalue);
                        pressureTxt.setText(String.format("%.1f hPa",pressure));
                    }
                });
                Log.d(TAG, String.format("Update pressure: %fhPa",pressure));
            }
        }

        /******************* Callback called when descriptor is written ***************************/
        /**** Indicates the result of the operation and deals with the descriptor write queue. ****/

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            Log.d(TAG, "GATT onDescriptorWrite()");

            if (status == BluetoothGatt.GATT_SUCCESS) {
                Log.d(TAG, "GATT: Descriptor wrote successfully."); //Operation ended successfully.
            } else {
                Log.d(TAG, "GATT: Error writing descriptor (" + status + ").");
            }

            descriptorWriteQueue.remove();

            if (descriptorWriteQueue.size() > 0) {
                mGatt.writeDescriptor(descriptorWriteQueue.element());
            }
        }

    }; // End of BluetoothGattCallback


    /************************** Method to write a descriptor. *************************************/

    public void writeGattDescriptor(BluetoothGattDescriptor descriptor) {
        descriptorWriteQueue.add(descriptor);

        Log.d(TAG, "Subscribed to " + descriptorWriteQueue.size() + " notification/s");

        try {
            if (descriptorWriteQueue.size() == 1)
                mGatt.writeDescriptor(descriptor);
        } catch (Exception e) {
            e.getLocalizedMessage();
        }
    }

    /**
     * Retrieves a list of supported GATT services on the connected device. This should be
     * invoked only after {@code BluetoothGatt#discoverServices()} completes successfully.
     *
     * @return A {@code List} of supported services.
     */
    public List<BluetoothGattService> getSupportedGattServices() {
        if (mGatt == null)
            return null;

        return mGatt.getServices();
    }
}

