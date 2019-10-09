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


namespace Auto_Besturing.Droid
{
    public class BTConnection
    {
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
                    object device = intent.GetParcelableExtra(BluetoothDevice.ExtraDevice);
                    Log.Info("friendly", "Device: " + string.Format("",device));
                    BTConnection.Devices.Add(BTConnection.bluetoothAdapter.GetRemoteDevice(string.Format("",device)));
                }
                Log.Info("friendly", "Action Registered: " + intent.Action);
            }
        }

        public class mScanFallback : Android.Bluetooth.LE.ScanCallback
        {
            public mScanFallback()
            {

            }
        }

        public MainPage Actpage;
        public static BroadcastReceiver mbroadcastReceiver1;
        public IntentFilter DeviceFoundFilter = new IntentFilter(BluetoothDevice.ActionFound);
        public bool IsActive { get; private set; }
        public Queue<int> SendData = new Queue<int>();
        public static BluetoothAdapter bluetoothAdapter;
        public BluetoothDevice ConnectedDevice;
        BluetoothSocket socket;
        public static List<BluetoothDevice> Devices;
        public Stream OutStream;
        public BTConnection(MainPage page)
        {
            Actpage = page;

            bluetoothAdapter = BluetoothAdapter.DefaultAdapter;
            Devices = new List<BluetoothDevice>();
            //Devices = new List<BluetoothDevice>(bluetoothAdapter.BondedDevices);

            //create broadcast receiver and register it
            mbroadcastReceiver1 = new mBroadcastReceiver1();
            Application.Context.RegisterReceiver(mbroadcastReceiver1, DeviceFoundFilter);

            //start discovery if not already discovering
            

            //set placeholder for ConnectedDevice
            if (Devices != null && Devices.Count > 0)
                ConnectedDevice = Devices[0];

            if (bluetoothAdapter.IsDiscovering)
            {
                bluetoothAdapter.CancelDiscovery();
            }
            bluetoothAdapter.BluetoothLeScanner.StartScan(new mScanFallback());
            if (bluetoothAdapter.StartDiscovery())
                Log.Info("friendly", "Discovering: " + bluetoothAdapter.IsDiscovering);
        }
        public BTConnection(string MacAdress)
        {
            bluetoothAdapter = BluetoothAdapter.DefaultAdapter;
            ConnectedDevice = bluetoothAdapter.GetRemoteDevice(MacAdress);
            if (!Initiate())
            {
                IsActive = false;
            }
        }
        public BTConnection(MainPage page, int Number)
        {
            Actpage = page;

            bluetoothAdapter = BluetoothAdapter.DefaultAdapter;
            Devices = new List<BluetoothDevice>(bluetoothAdapter.BondedDevices);
            /*if (bluetoothAdapter.IsDiscovering)
            {
                bluetoothAdapter.CancelDiscovery();
            }
            bluetoothAdapter.StartDiscovery();

            try
            {
                IntentFilter DiscoverIntentFilter = new IntentFilter(BluetoothDevice.ActionFound);
                Intent DiscoverIntent = new Intent(BluetoothDevice.ActionFound);
                Devices.Add(DiscoverIntent.GetParcelableExtra(BluetoothDevice.ExtraDevice) as BluetoothDevice);
            }
            catch (Exception e)
            {
                throw e;
            }*/
            if (Devices != null && Devices.Count > 0)
                ConnectedDevice = Devices[Number];

            if (!Initiate())
            {
                IsActive = false;
            }
        }

        //General initiation for the class
        public bool Initiate()
        {
            if (ConnectedDevice != null)
            {
                IsActive = true;
                try
                {
                    ParcelUuid[] list = ConnectedDevice.GetUuids(); //Get Device UUID
                    socket = ConnectedDevice.CreateInsecureRfcommSocketToServiceRecord(Java.Util.UUID.FromString(list[0].ToString())); //Create Socket with UUID
                    Log.Info("friendly", "Device: " + ConnectedDevice.Name + " // " + list[0].ToString());

                }
                catch (System.Exception e)
                {
                    Log.Error("friendly", "create() failed", e);
                    Actpage.DisplayAlert("UUID retrieval failed", "Could not retrieve UUID from device", "Ok");
                    return false;
                }
                

                //Log.Info("friendly", "Device: " + ConnectedDevice + " // " + socket);

               try
                {
                    // This is a blocking call and will only return on a
                    // successful connection or an exception
                    socket.Connect();
                }
                catch (Exception e)
                {
                    // Close the socket
                    try
                    {
                        socket.Close();
                        Actpage.DisplayAlert("Connecting failed", "Could not establish connection between device", "Ok");
                    }
                    catch (Exception e2)
                    {
                        Log.Error("friendly", $"unable to close() {socket.ConnectionType} socket during connection failure.", e2);
                        Actpage.DisplayAlert("Fatal Error", "A Fatal error occured while running an operation", "Ok");
                    }
                    return false; // Could Not Establish connection. Try again by invoking Initiate
                }
                    OutStream = socket.OutputStream;
                Thread connectedThread = new Thread(new ThreadStart(RunThread));
                connectedThread.Start();
            }
            else
            {
                IsActive = false;
                MainActivity.CloseApplication("Could not Find Bluetooth", "Please Enable Bluetooth", "Ok", bluetoothAdapter);

            }
            return true;
        }

        public Stream InStream;
        public void RunThread()
        {
            if (MainPage.BtConnection == this)
            {
                InStream = socket.InputStream;
                while (true)
                {
                    //Log.Info("friendly", "MotorData: " + "testdata");
                    //InStream.ReadByte();
                    if (SendData.Count > 0)
                    {
                        int send = SendData.Dequeue();
                        //Log.Info("friendly", "MotorData: " + send);
                        WriteInt(send * 16);
                    }
                    else
                    {
                        //WriteInt(1);
                    }
                }
            }
            else return;
            


        }


        public List<string> GetDeviceNames()
        {
            List<string> str = new List<string>();
            for (int i = 0; i < Devices.Count; i++)
            {
                if (Devices[i] != null)
                {
                    str.Add(Devices[i].Name);
                }
            }
            return str;
        }

        public void WriteByte(Byte[] data)
        {
            OutStream.Write(data, 0, 1);
        }
        public void WriteInt(int data)
        {
            //if (OutStream != null)
            //{
                Byte[] OutBytes = BitConverter.GetBytes(data);
                //Log.Info("friendly", "OutBytes: " + OutStream);
                OutStream.Write(OutBytes, 0, OutBytes.Length);
            //}
        }
    }

    
}