using System;
using System.Collections;
using System.IO.Ports;
using System.IO;
using System.Data;
using System.Net;
/*using InTheHand;
using InTheHand.Net;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Ports;
using InTheHand.Net.Sockets;
using System.Threading;



namespace BTFrame
{
    class BTConnector
    {

        Byte[] received = new byte[1024];
        public Guid guid = new Guid("00001011-0000-1000-8000-08005F9B34FB"); //guid
        BluetoothListener BluetoothList;
        BluetoothClient client = null;

        public BTConnector()
        {
            try
            {
                BluetoothList = new BluetoothListener(guid);
                BluetoothList.Start();
                Thread ConnectThread = new Thread(ListenToClient);
            }
            catch (Exception e)
            {
                if (e.GetType() == typeof(PlatformNotSupportedException))
                {
                    Console.WriteLine("Could not perform operation, No Bluetooth support found");
                }
                else
                    throw e;
                
            }

        }
        void ListenToClient()
        {
            client = BluetoothList.AcceptBluetoothClient();
            Stream BTstream = client.GetStream();
            while (true)
            {
                BTstream.Read(received, 0, received.Length);
            }
        }
        public Byte[] GetData()
        {
            return received;
        }
        public int SendData(Byte[] data)
        {
            if (client != null)
            {
                Stream BTstream = client.GetStream();
                BTstream.Write(data, 0, data.Length);
                return 0;
            }
            return 1;
        }
    }
}*/