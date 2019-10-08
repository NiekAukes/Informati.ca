using Auto_Besturing.Droid;
using System;
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
        //BTConnection BtConnection = new BTConnection("40:A3:CC:03:E1:29");
        public static BTConnection BtConnection;
        public MainPage()
        {
            //Bluetooth Initialization
            InitializeComponent();
            BtConnection = new BTConnection(this);

            if (BtConnection.GetDeviceNames().Count > 0)
                DevicePick.Items.Add(BtConnection.GetDeviceNames()[0]);
            for (int i = 1; i < BtConnection.GetDeviceNames().Count; i++)
            {
                DevicePick.Items.Add(BtConnection.GetDeviceNames()[i]);
            }

            DisplayLog displayLog = new DisplayLog(500);
            displayLog.LogEntry("test entry");

            Android.Util.Log.Info("friendly", "BBuildit"+ 527);
            Thread UpdateThread = new Thread(new ThreadStart(WhileActive));
            UpdateThread.Start();

        }

        public void WhileActive()
        {
            int times = 0;
            while (true)
            {
                try
                {
                    times++;

                    //Sends Data Corresponding to The Send Queue
                    if (BtConnection.SendData.Count < 40)
                    {

                        if (Up)
                            BtConnection.SendData.Enqueue((int)BTCodeOut.Forward);
                        else if (Down)
                            BtConnection.SendData.Enqueue((int)BTCodeOut.Backward);
                        else if (Left)
                            BtConnection.SendData.Enqueue((int)BTCodeOut.Left);
                        else if (Right)
                            BtConnection.SendData.Enqueue((int)BTCodeOut.Right);
                        else
                            BtConnection.SendData.Enqueue((int)BTCodeOut.Stop);


                    }

                    Thread.Sleep(5);

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

        private void DevicePick_SelectedIndexChanged(object sender, EventArgs e)
        {
            selectedIndex = ((Picker)sender).SelectedIndex - 1;
            Android.Util.Log.Debug("friendly", "New Bt Device: " + BtConnection.ConnectedDevice.Name);
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            BtConnection = new BTConnection(this, selectedIndex);
        }
    }

    class DisplayLog : Entry
    {
        string[] LogEntries = new string[500];
        public DisplayLog(double height, bool isEnabled = true, bool isReadOnly = true)
        {
            IsEnabled = isEnabled;
            IsReadOnly = isReadOnly;
            HeightRequest = height;
        }
        public void ShowLog()
        {
            IsEnabled = false;
        }
        public void LogEntry(object entry, int sendcode = 0)
        {
            bool EnteredLog = false;
            for (int i = 0;i < LogEntries.Length; i++)
            {
                if (LogEntries[i] == null)
                {
                    EnteredLog = true;
                    LogEntries[i] = string.Format("", entry);
                } 
            }
            if (!EnteredLog)
            {
                //Log list is full
                for (int i = 0; i < LogEntries.Length - 1; i++)
                {
                    LogEntries[i] = LogEntries[i + 1];
                }
                LogEntries[500] = string.Format("", entry);
            }
            this.Text = string.Format("", LogEntries);
        }
    }
}
