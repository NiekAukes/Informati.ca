using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;

using Android.App;
using Android.Content;
using Android.Content.Res;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Bluetooth;
using Android.Net;
using Android.Util;
using Android.Bluetooth.LE;

namespace Auto_Besturing.Droid
{

    

    public class BTService
    {
        #region BroadCastReceivers
        [BroadcastReceiver(Enabled = true, Label = "receiver")]
        public class mBroadcastReceiver1 : BroadcastReceiver
        {
            public mBroadcastReceiver1()
            {
                Log.Info("friendly", "Created Listener");
            }
            public override void OnReceive(Context context, Intent intent)
            {
                if (BluetoothDevice.ActionFound == intent.Action)
                {
                    //bluetoothdevice found
                    bool addDevice = true;

                    object device = intent.GetParcelableExtra(BluetoothDevice.ExtraDevice);
                    BluetoothDevice detectedDevice = Adapter.GetRemoteDevice(device.ToString());

                    foreach (BluetoothDevice bluetoothDevice in BTDevices)
                    {
                        if (detectedDevice.Address == bluetoothDevice.Address)
                        {
                            addDevice = false;
                        }
                    }
                    foreach (BluetoothDevice bluetoothDevice in BLEDevices)
                    {
                        if (detectedDevice.Address == bluetoothDevice.Address)
                        {
                            addDevice = false;
                        }
                    }
                    if (addDevice)
                    {
                        BTService.BLEDevices.Add(detectedDevice);
                        Log.Info("friendly", "Device: " + detectedDevice.Name);
                    }
                }
                Log.Info("friendly", "Action Registered: " + intent.Action);
            }
        }

        [BroadcastReceiver(Enabled = true, Label = "receiver")]
        public class mGattBroadcastReceiver : BroadcastReceiver
        {
            public override void OnReceive(Context context, Intent intent)
            {
               

            }
        }


        public class mScanFallback : Android.Bluetooth.LE.ScanCallback
        {
            public override void OnScanResult([GeneratedEnum] ScanCallbackType callbackType, ScanResult result)
            {
                base.OnScanResult(callbackType, result);
                bool addDevice = true;
                foreach(BluetoothDevice device in BLEDevices)
                {
                    if (device.Address == result.Device.Address)
                    {
                        addDevice = false;
                    }
                }
                if (addDevice)
                {
                    BLEDevices.Add(result.Device);
                }

                //MainActivity.appl.MainPage.DisplayAlert("Found", "Found a Device", "Ok");
                
            }
        }

        

        public class mGattCallback : Android.Bluetooth.BluetoothGattCallback
        {
            public mGattCallback()
            {

            }
            public override void OnConnectionStateChange(BluetoothGatt gatt, [GeneratedEnum] GattStatus status, [GeneratedEnum] ProfileState newState)
            {
                base.OnConnectionStateChange(gatt, status, newState);

                switch (newState)
                {
                    case ProfileState.Connected:
                        Log.Debug("friendly", "Now Connected");

                        MainActivity.appl.MainPage.DisplayAlert("Connected", "Connected to the Device", "Ok");
                        break;
                    case ProfileState.Connecting:
                        Log.Debug("friendly", "Now Connecting");
                        break;
                    case ProfileState.Disconnected:
                        Log.Debug("friendly", "Now Disconnected");
                        break;
                    case ProfileState.Disconnecting:
                        Log.Debug("friendly", "Now Disconnecting");

                        MainActivity.appl.MainPage.DisplayAlert("Disconnected", "Remote Device Broke the connection", "Ok");
                        break;
                }



            }
            public override void OnServicesDiscovered(BluetoothGatt gatt, [GeneratedEnum] GattStatus status)
            {
                base.OnServicesDiscovered(gatt, status);
            }
        }

        public class mGattServerCallback : Android.Bluetooth.BluetoothGattServerCallback
        {
            public override void OnCharacteristicReadRequest(BluetoothDevice device, int requestId, int offset, BluetoothGattCharacteristic characteristic)
            {
                base.OnCharacteristicReadRequest(device, requestId, offset, characteristic);
            }
        }

        public class mAdvertiseCallback : AdvertiseCallback
        {
            public override void OnStartSuccess(AdvertiseSettings settingsInEffect)
            {
                base.OnStartSuccess(settingsInEffect);
                activeService.StartServer();
            }
        }
        #endregion

        public static BluetoothManager Manager;
        public static BluetoothAdapter Adapter;
        public static BTService activeService;
        public static List<BluetoothDevice> BLEDevices = new List<BluetoothDevice>();
        public static List<BluetoothDevice> BTDevices = new List<BluetoothDevice>();

        public mBroadcastReceiver1 BTReceiver = new mBroadcastReceiver1();
        public mScanFallback BLEReceiver = new mScanFallback();
        public mAdvertiseCallback advertiseCallback = new mAdvertiseCallback();
        public mGattServerCallback gattServerCallback = new mGattServerCallback();

        public BluetoothGattServer gattServer;
        public BluetoothGatt gatt;
        public Handler handler = new Handler();

        public bool GattServerActive = false;

        public byte[] ServiceBytes;
        public BTService()
        {
            Manager = (BluetoothManager)Application.Context.GetSystemService(Java.Lang.Class.FromType(typeof(BluetoothManager)));
            activeService = this;
            Adapter = BluetoothAdapter.DefaultAdapter;
            if (Adapter == null) //no BT support
            {
                throw new Exception("Bluetooth not available on this device");
            }
            if (!Adapter.IsEnabled)
            {
                //if bluetooth is not enabled, ask for enable
                Application.Context.SendBroadcast(new Intent(BluetoothAdapter.ActionRequestEnable));
            }

            IntentFilter BTFilterFound = new IntentFilter(BluetoothDevice.ActionFound);
            Application.Context.RegisterReceiver(BTReceiver, BTFilterFound);
            
        }
        public void StartLESearch()
        {
            //BLEDevices.Clear();
            Adapter.BluetoothLeScanner.StartScan(BLEReceiver);
        }
        public void StartClassicSearch()
        {
            throw new NotImplementedException();
        }

        public void ConnectGatt(BluetoothDevice device)
        {
            mGattCallback bluetoothGattCallback = new mGattCallback();
            gatt = device.ConnectGatt(Application.Context, false, bluetoothGattCallback, BluetoothTransports.Le, ScanSettingsPhy.AllSupported, handler);
            gatt.DiscoverServices();
        }

        public void StartLEServer()
        {
            ParcelUuid serviceUUID = new ParcelUuid(Java.Util.UUID.FromString("fee2a702-ee95-11e9-81b4-2a2ae2dbcce4"));
            

            AdvertiseSettings.Builder advertiseSettingsBuilder = new AdvertiseSettings.Builder();
            advertiseSettingsBuilder.SetTimeout(0);
            advertiseSettingsBuilder.SetAdvertiseMode(AdvertiseMode.Balanced);
            advertiseSettingsBuilder.SetTxPowerLevel(AdvertiseTx.PowerMedium);

            AdvertiseData.Builder advertiseDataBuilder = new AdvertiseData.Builder();
            advertiseDataBuilder.SetIncludeDeviceName(true);
            advertiseDataBuilder.AddServiceUuid(serviceUUID);
            if (ServiceBytes != null)
                advertiseDataBuilder.AddServiceData(serviceUUID, ServiceBytes);

            Adapter.BluetoothLeAdvertiser.StartAdvertising(advertiseSettingsBuilder.Build(), advertiseDataBuilder.Build(), advertiseCallback);
        }

        private void StopAdvertising()
        {
            Adapter.BluetoothLeAdvertiser.StopAdvertising(advertiseCallback);
        }

        private void StartServer()
        {
            gattServer = Manager.OpenGattServer(Application.Context, gattServerCallback);
            if (gattServer != null)
            {
                GattServerActive = true;
                MainActivity.appl.MainPage.DisplayAlert("Gattserver Success", "Gatt Server was created succesfully", "Ok");
            }
            else
            {
                GattServerActive = false;
                throw new NullReferenceException("Could not Create Server");
            }
        }

        public void ConnectRFCOMM()
        {
            throw new NotImplementedException();
        }

        public void OnDestroy()
        {
            activeService = null;
        }
    }

}