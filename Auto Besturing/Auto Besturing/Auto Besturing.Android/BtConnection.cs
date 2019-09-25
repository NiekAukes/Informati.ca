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
    class BTConnection
    {
        public bool IsActive { get; private set; }
        public Queue<int> SendData = new Queue<int>();
        public BluetoothAdapter bluetoothAdapter;
        BluetoothDevice ConnectedDevice;
        BluetoothSocket socket;
        List<BluetoothDevice> Devices;
        public Stream OutStream;
        public BTConnection()
        {
            bluetoothAdapter = BluetoothAdapter.DefaultAdapter;
            Devices = new List<BluetoothDevice>(bluetoothAdapter.BondedDevices);
            if (Devices != null && Devices.Count > 0)
                ConnectedDevice = Devices[0];

            foreach (BluetoothDevice d in Devices)
            {
                Log.Info("friendly", "Device: " + d);
            }
            if (!Initiate())
            {
                IsActive = false;
            }
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

        //General initiation for the class
        bool Initiate()
        {
            if (ConnectedDevice != null)
            {
                IsActive = true;
                try
                {
                    ParcelUuid[] list = ConnectedDevice.GetUuids(); //Get Device UUID
                    socket = ConnectedDevice.CreateInsecureRfcommSocketToServiceRecord(Java.Util.UUID.FromString(list[0].ToString())); //Create Socket with UUID
                    Log.Info("friendly", "Device: " + ConnectedDevice + " // " + list[0].ToString());

                }
                catch (Java.IO.IOException e)
                {
                    Log.Error("friendly", "create() failed", e);
                }

                Log.Info("friendly", "Device: " + ConnectedDevice + " // " + socket);

               try
                {
                    // This is a blocking call and will only return on a
                    // successful connection or an exception
                    socket.Connect();
                }
                catch (Java.IO.IOException e)
                {
                    // Close the socket
                    try
                    {
                        socket.Close();
                    }
                    catch (Java.IO.IOException e2)
                    {
                        Log.Error("friendly", $"unable to close() {socket.ConnectionType} socket during connection failure.", e2);
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
                MainActivity.CloseApplication("Could not Find Bluetooth", "Please Enable Bluetooth", "Ok");

            }
            return true;
        }

        public Stream InStream;
        public void RunThread()
        {
            InStream = socket.InputStream;
            while (true)
            {
                //Log.Info("friendly", "MotorData: " + "testdata");
                //InStream.ReadByte();
                if (SendData.Count > 0)
                {
                    int send = SendData.Dequeue();
                    Log.Info("friendly", "MotorData: " + send);
                    WriteInt(send);
                }
                else
                {
                    //WriteInt(1);
                    Log.Info("friendly", "MotorData: " + 0);
                }
                Thread.Sleep(10);
            }
            


        }

        public string[] GetDeviceNames()
        {
            string[] str = new string[100];
            for (int i = 0; i < Devices.Count; i++)
            {
                str[i] = Devices[i].Name;
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
                Log.Info("friendly", "OutBytes: " + OutStream);
                OutStream.Write(OutBytes, 0, OutBytes.Length);
            //}
        }
    }
}