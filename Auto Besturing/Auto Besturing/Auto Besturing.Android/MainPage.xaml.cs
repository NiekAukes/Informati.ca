using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using Android;
using Android.App;
using Xamarin.Forms;
using Auto_Besturing.Droid;
//using BTFrame;

namespace Auto_Besturing
{
    // Learn more about making custom code visible in the Xamarin.Forms previewer
    // by visiting https://aka.ms/xamarinforms-previewer
    [DesignTimeVisible(false)]
    public partial class MainPage : ContentPage
    {

        
        public bool Left = false;
        public bool Right = false;
        public bool Up = false;
        public bool Down = false;
        //When a value is between -50 & -25 or 25 & 50, the value is locked.
        public int[] MotorData = { 0, 0, 0, 0 }; // {LeftUp, LeftDown, RightUp, RightDown}
        //BTConnection BtConnection = new BTConnection("40:A3:CC:03:E1:29");
        BTConnection BtConnection;
        public MainPage()
        {
            //Bluetooth Initialization
            InitializeComponent();
            BtConnection = new BTConnection();
            Android.Util.Log.Info("friendly", "BBuildit"+ 527);
            Thread UpdateThread = new Thread(new ThreadStart(WhileActive));
            UpdateThread.Start();

            //Page Initialization
            DevicePick.Items.Add("None");
            foreach (string s in BtConnection.GetDeviceNames())
            {
                DevicePick.Items.Add(s);
            }
        }

        public void WhileActive()
        {
            int times = 0;
            while (true)
            {
                Android.Util.Log.Info("friendly", "MotorData: " + string.Join(", ", MotorData));

                try
                {
                    if (!BtConnection.IsActive)
                    {
                        BtConnection = new BTConnection();
                    }
                    times++;
                    MotorData = new int[4] { 0, 0, 0, 0 };
                    if (Left)
                    {
                        //lock Left Motors to 0
                        //MotorData[0] = 25;
                        MotorData[1] = 25;
                    }
                    if (Right)
                    {
                        //lock Right Motors to 0
                        //MotorData[2] = 25;
                        MotorData[3] = 25;
                    }
                    for (int i = 0; i < 4; i++)
                    {
                        if (MotorData[i] > -25 && MotorData[i] < 25 && (Up || Down)) // if forward or backward and not locked
                        {
                            MotorData[i] = 24;
                        }
                        else // if locked or not forward/backward
                        {
                            MotorData[i] = 0;
                        } 

                        MotorData[i] = Down ? -MotorData[i] : MotorData[i];
                    }

                    if (BtConnection.SendData.Count < 40)
                    {
                        //MotorData Send
                        for (int i = 0; i < 4; i++)
                        {
                            BtConnection.SendData.Enqueue(MotorData[i] % 25 + 25 + 56 + 50 * i);
                            //MotorData[i] % 25 (Get value between -24 and 24) + 24 (no negatives) + 56 (Async Offset for motors) + 50 * i (Individual Motor offset)
                        }
                    }
                    Thread.Sleep(10);

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
    }
}
