using Auto_Besturing.Droid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Threading;
using Xamarin.Forms;
//using BTFrame;

namespace Auto_Besturing
{
    // Learn more about making custom code visible in the Xamarin.Forms previewer
    // by visiting https://aka.ms/xamarinforms-previewer
    [DesignTimeVisible(false)]
    public partial class MainPage : ContentPage
    {
        enum BTCodeOut
        {
            Null,
            Faulty,
            AutoSetup,
            ManualSetup,
            Stop,
            Forward,
            Backward,
            Left,
            Right,
            AccelerationRange = 20, //To 120
            TBD = 121
        }
        enum BTCodeIn
        {
            Null,
            Faulty,
            ServoAngleRange=2, // To 32
            MeterValueRange=33, // To 232
            TBD = 233
        }

        public int selectedIndex = 0;
        public float Speed = 0.0f;
        public bool Left = false;
        public bool Right = false;
        public bool Up = false;
        public bool Down = false;
        BTService testService;
        public MainPage()
        {
            //Bluetooth Initialization
            InitializeComponent();

            

            testService = new BTService();
            testService.StartLESearch();

            

            Android.Util.Log.Info("friendly", "BBuildit"+ 527);
            Thread UpdateThread = new Thread(new ThreadStart(WhileActive));
            UpdateThread.Start();

        }

        public void WhileActive()
        {
            int times = 0;
            while (true)
            {
                //Android.Util.Log.Debug("friendly", "Got here");
                try
                {
                    times++;

                    //Sends Data Corresponding to The Send Queue
                    if (testService.gatt != null)
                    {

                        if (Up)
                        { //BtConnection.SendData.Enqueue((int)BTCodeOut.Forward);
                            testService.handler.DispatchMessage(new Android.OS.Message { Arg1 = 52 });
                            Android.Util.Log.Debug("friendly", "Forward Sent");
                        }
                       
                        if (testService.gatt.Services.Count > 0)
                        {
                            int getcontent = testService.gatt.Services[0].Characteristics[0].DescribeContents();
                            Android.Util.Log.Debug("friendly", "Content: " + getcontent);
                        }

                    }

                    Thread.Sleep(50);

                }
                catch (Exception e)
                {
                    throw new Exception(e.StackTrace);
                }
            }


        }


        #region PressHandlers
        void LeftPressHandler(object sender, EventArgs e)
        {
           
            Left = true;
        }
        void LeftReleaseHandler(object sender, EventArgs e)
        {
            Left = false;
        }
        void RightPressHandler(object sender, EventArgs e)
        {
            Right = true;
        }
        void RightReleaseHandler(object sender, EventArgs e)
        {
            Right = false;
        }
        void UpPressHandler(object sender, EventArgs e)
        {
            Up = true;
        }
        void UpReleaseHandler(object sender, EventArgs e)
        {
            Up = false;
        }
        void DownPressHandler(object sender, EventArgs e)
        {
            Down = true;
        }
        void DownReleaseHandler(object sender, EventArgs e)
        {
            Down = false;
        }
        #endregion PressHandlers

        List<string> devicenames = new List<string>();
        private void RefreshBTAvailable(object sender, EventArgs e)
        {
            testService.StartLESearch();
            devicenames.Clear();
            devicenames.Add("None");
            foreach (Android.Bluetooth.BluetoothDevice bluetoothDevice in BTService.BLEDevices)
            {
                if (bluetoothDevice.Name != null)
                {
                    devicenames.Add(bluetoothDevice.Name);
                    Android.Util.Log.Debug("friendly", "Device Refresh: " + bluetoothDevice.Name);
                }
                else
                {
                    devicenames.Add(bluetoothDevice.Address);
                    Android.Util.Log.Debug("friendly", "Device Refresh: " + bluetoothDevice.Address);
                }

            }
            DevicePick.ItemsSource = devicenames;
        }

        private void DevicePick_SelectedIndexChanged(object sender, EventArgs e)
        {
            selectedIndex = ((Picker)sender).SelectedIndex - 1;
            Android.Util.Log.Debug("friendly", "New Bt Device: " + ((Picker)sender).Items[selectedIndex + 1]);
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            if (selectedIndex >= 0)
            {
                testService.ConnectGatt(BTService.BLEDevices[selectedIndex]);
                //testService.StartLEServer();
            } else
            {
                DisplayAlert("No device Selected", "No Device was selected, please select a valid device", "Ok");
            }

        }
    }

}
