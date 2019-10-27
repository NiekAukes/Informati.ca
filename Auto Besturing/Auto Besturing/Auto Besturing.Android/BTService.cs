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
using System.Threading.Tasks;

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
                    MainPage.ActivePage.RefreshBT();
                }

                //MainActivity.appl.MainPage.DisplayAlert("Found", "Found a Device", "Ok");
                
            }
        }

        

        public class mGattCallback : Android.Bluetooth.BluetoothGattCallback
        {
            public mGattCallback()
            {

            }
            public override void OnDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, [GeneratedEnum] GattStatus status)
            {
                base.OnDescriptorRead(gatt, descriptor, status);
                MainPage.LogEntry(descriptor.DescribeContents().ToString());
            }
            public override void OnCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic)
            {
                base.OnCharacteristicChanged(gatt, characteristic);
                Log.Debug("friendly","Got a new Value: " + characteristic.GetStringValue(0));
            }
            public override void OnCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, [GeneratedEnum] GattStatus status)
            {
                base.OnCharacteristicRead(gatt, characteristic, status);
                if (characteristic.GetStringValue(0) != null)
                {
                    MainPage.LogEntry("Got a new read Value: " + characteristic.GetStringValue(0));
                }
            }
            public override void OnCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, [GeneratedEnum] GattStatus status)
            {
                base.OnCharacteristicWrite(gatt, characteristic, status);
                MainPage.LogEntry("Write Value: " + characteristic.GetIntValue(GattFormat.Sint16, 0).ToString());
            }
            public override void OnReliableWriteCompleted(BluetoothGatt gatt, [GeneratedEnum] GattStatus status)
            {
                base.OnReliableWriteCompleted(gatt, status);
                MainPage.LogEntry("Reliable Write Completed");
            }
            public override void OnConnectionStateChange(BluetoothGatt gatt, [GeneratedEnum] GattStatus status, [GeneratedEnum] ProfileState newState)
            {
                base.OnConnectionStateChange(gatt, status, newState);

                switch (newState)
                {
                    case ProfileState.Connected:
                        MainPage.LogEntry("Now Connected");
                        gatt.DiscoverServices();
                        MainActivity.appl.MainPage.DisplayAlert("Connected", "Connected to the Device", "Ok");
                        break;
                    case ProfileState.Connecting:
                        MainPage.LogEntry("Now Connecting");
                        break;
                    case ProfileState.Disconnected:
                        MainPage.LogEntry("Now Disconnected");
                        MainActivity.appl.MainPage.DisplayAlert("Disconnected", "Remote Device Broke the connection", "Ok");
                        break;
                    case ProfileState.Disconnecting:
                        MainPage.LogEntry("Now Disconnecting");
                        break;
                }



            }
            public override void OnServicesDiscovered(BluetoothGatt gatt, [GeneratedEnum] GattStatus status)
            {
                base.OnServicesDiscovered(gatt, status);
                MainPage.LogEntry("Discovered Services");
                activeService.servicepeek = new List<BluetoothGattService>(gatt.Services);

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
        public BluetoothGattService GattService;
        public BluetoothGattCharacteristic characteristicWrite;
        public BluetoothGattCharacteristic characteristicRead;
        public Handler handler = new Handler();
        public static Java.Util.UUID ServiceUUID = Java.Util.UUID.FromString("0000ffe0-0000-1000-8000-00805f9b34fb");
        public static Java.Util.UUID CharacteristicUUIDWrite = Java.Util.UUID.FromString("0000ffe1-0000-1000-8000-00805f9b34fb");
        public static Java.Util.UUID CharacteristicUUIDRead = Java.Util.UUID.FromString("0000ffe1-0000-1000-8000-00805f9b34fb");

        public bool GattServerActive = false;
        List<BluetoothGattService> servicepeek;

        public byte[] ServiceBytes;
        public BTService()
        {
            Manager = (BluetoothManager)Application.Context.GetSystemService(Java.Lang.Class.FromType(typeof(BluetoothManager)));
            activeService = this;
            Adapter = BluetoothAdapter.DefaultAdapter;
            if (Adapter == null) //no BT support
            {
                return;
                //throw new Exception("Bluetooth not available on this device");
            }
            if (!Adapter.IsEnabled)
            {
                //if bluetooth is not enabled, ask for enable
                Application.Context.SendBroadcast(new Intent(BluetoothAdapter.ActionRequestEnable));
                MainPage.LogEntry("Bluetooth not enabled");
            }

            IntentFilter BTFilterFound = new IntentFilter(BluetoothDevice.ActionFound);
            Application.Context.RegisterReceiver(BTReceiver, BTFilterFound);

            StartLESearch();
            
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

        public bool ConnectGatt(BluetoothDevice device)
        {
            mGattCallback bluetoothGattCallback = new mGattCallback();
            MainPage.LogEntry(device.CreateBond().ToString());
            gatt = device.ConnectGatt(Application.Context, false, bluetoothGattCallback, BluetoothTransports.Auto, ScanSettingsPhy.AllSupported, handler);
            if (gatt != null)
            {
                gatt.DiscoverServices();
                GattService = gatt.GetService(ServiceUUID);
                return true;
            }
            return false;

            //GattService = new BluetoothGattService(ServiceUUID, GattServiceType.Primary);
            
            //characteristicWrite = new BluetoothGattCharacteristic(CharacteristicUUIDWrite, GattProperty.Notify, GattPermission.Read);
            //GattService.AddCharacteristic(characteristicWrite);
            //characteristicRead = new BluetoothGattCharacteristic(CharacteristicUUIDWrite, GattProperty.Notify, GattPermission.Write);
            //GattService.AddCharacteristic(characteristicRead);
            //gatt.BeginReliableWrite();
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
        public bool WriteGattCharacteristic(int value)
        {
            /*
             * Write Edge Cases:
             * 
             */

            if (gatt == null) //no gatt connection was active, human error
            {
                new ErrorHandler("Characteristic cannot be null", MainActivity.appl.MainPage);
                return false;
            }
            GattService = gatt.GetService(ServiceUUID);
            if (GattService == null)
            {
                new ErrorHandler("Could not Send to the service", MainActivity.appl.MainPage);
                return false;
            }
            BluetoothGattCharacteristic gattCharacteristic = GattService.GetCharacteristic(CharacteristicUUIDWrite);
            if (gattCharacteristic == null)
            {
                new ErrorHandler("Could not Send to the characteristic", MainActivity.appl.MainPage);
                return false;
            }
            gattCharacteristic.SetValue(BitConverter.GetBytes(value));
            bool f = gatt.WriteCharacteristic(GattService.GetCharacteristic(CharacteristicUUIDWrite));
            


            //gatt.ReadCharacteristic(GattService.GetCharacteristic(CharacteristicUUIDRead));
            return f; //operation succesfully completed
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

        //private Task StartServerAsync

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