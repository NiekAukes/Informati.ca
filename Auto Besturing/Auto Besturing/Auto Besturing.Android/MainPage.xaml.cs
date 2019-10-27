using Auto_Besturing.Droid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Threading;
using Xamarin.Forms;
using AudioPlayEx.Droid;
using System.Threading.Tasks;
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
            Null,//00 hex
            Faulty,//01 hex
            Manual,//02 hex
            Auto,//03 hex
            Stop,//04 hex
            Forward,//05 hex
            Backward,//06 hex
            Left,//07 hex
            Right,//08 hex
            ForwardLeft, //09hex
            ForwardRight = 11, //0B hex
            CheckDistance = 249,//for distance meter, hex F9
            Servo20deg = 250,//hex FA
            Servo60deg = 251,//hex FB
            Servo90deg = 252,//hex FC
            Servo120deg = 253,//hex FD
            Servo160deg = 254,//hex FE
            CheckAllAngles = 15, //hex 0F
            BeginAccelerationRange = 20,
            EndAccelerationRange = 120,
            BeginSteeringRange = 130,
            EndSteeringRange = 230
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
        public bool OnChanged = false;
        BTService testService;
        public static MainPage ActivePage;
        public static Editor Logger;
        public MainPage()
        {
            //Bluetooth Initialization
            InitializeComponent();


            ActivePage = this;
            //playButton.Margin = new Thickness(30.0);
            if (!DesignMode.IsDesignModeEnabled)
            {
                // Don't run in the Previewer  

                testService = new BTService();
                testService.StartLESearch();



                LogEntry("BBuildit" + 527);
                Thread UpdateThread = new Thread(new ThreadStart(WhileActive));
                UpdateThread.Start();
                LogEntry("Made Taco");

            }
            else
            {
                Image image = new Image()
                {
                    Source = TacoButton.Source
                };
                HeadLayout.Children.Add(image);
                AbsoluteLayout.SetLayoutFlags(image, AbsoluteLayoutFlags.PositionProportional);
                AbsoluteLayout.SetLayoutBounds(image, new Rectangle(0.5, 0.5, 100, 100));
                LogEntry("Made Taco");

            }
        }

        public void WhileActive()
        {
            int times = 0;
            while (true)
            {
                //LogEntry("Got here");
                try
                {
                    times++;

                    //Sends Data Corresponding to The Send Queue
                    if (testService.gatt != null)
                    {
                        if (testService.gatt.Services.Count > 0 && OnChanged)
                        {

                            if (Up)
                            { 
                                bool succes = testService.WriteGattCharacteristic((int)BTCodeOut.Forward);

                                LogEntry("Forward" + (succes ? "" : " not") + " Sent");
                            }
                            if (Left)
                            {
                                bool succes = testService.WriteGattCharacteristic((int)BTCodeOut.Left);

                                LogEntry("Left" + (succes ? "" : " not") + " Sent");
                            }
                            if (Right)
                            {
                                bool succes = testService.WriteGattCharacteristic((int)BTCodeOut.Right);

                                LogEntry("Right" + (succes ? "" : " not") + " Sent");
                            }

                            if (Down)
                            {
                                bool succes = testService.WriteGattCharacteristic((int)BTCodeOut.Backward);
                                Android.Bluetooth.BluetoothGattCharacteristic characteristic = testService.GattService.GetCharacteristic(BTService.CharacteristicUUIDRead);
                                testService.gatt.ReadCharacteristic(characteristic);
                            }
                            if (!Up && !Right && !Left && !Down)
                            {
                                bool succes = testService.WriteGattCharacteristic((int)BTCodeOut.Stop);
                            }
                            OnChanged = false;
                        }
                    }


                    Thread.Sleep(10);

                }
                catch (Exception e)
                {
                    throw e;
                }
            }


        }



        #region PressHandlers
        void LeftPressHandler(object sender, EventArgs e)
        {
            Left = true;
            OnChanged = true;
        }
        void LeftReleaseHandler(object sender, EventArgs e)
        {
            Left = false;
            OnChanged = true;
        }
        void RightPressHandler(object sender, EventArgs e)
        {
            Right = true;
            OnChanged = true;
        }
        void RightReleaseHandler(object sender, EventArgs e)
        {
            Right = false;
            OnChanged = true;
        }
        void UpPressHandler(object sender, EventArgs e)
        {
            Up = true;
            OnChanged = true;
        }
        void UpReleaseHandler(object sender, EventArgs e)
        {
            Up = false;
            OnChanged = true;
        }
        void DownPressHandler(object sender, EventArgs e)
        {
            Down = true;
            OnChanged = true;
        }
        void DownReleaseHandler(object sender, EventArgs e)
        {
            Down = false;
            OnChanged = true;
        }
        #endregion PressHandlers

        List<string> devicenames = new List<string>();
        private void RefreshBTAvailable(object sender, EventArgs e)
        {
            BTService.BLEDevices.Clear();
            testService.StartLESearch();
            RefreshBT();
        }
        public void RefreshBT()
        {
            DevicePick.Items.Clear();
            DevicePick.Items.Add("None");
            foreach (Android.Bluetooth.BluetoothDevice bluetoothDevice in BTService.BLEDevices)
            {
                if (bluetoothDevice.Name != null)
                {
                    DevicePick.Items.Add(bluetoothDevice.Name);
                    LogEntry("Device Refresh: " + bluetoothDevice.Name);
                }
                else
                {
                    DevicePick.Items.Add(bluetoothDevice.Address);
                    LogEntry("Device Refresh: " + bluetoothDevice.Address);
                }

            }
            //DevicePick.ItemsSource = devicenames;
        }


        private void DevicePick_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (((Picker)sender).SelectedIndex > -1)
            {
                selectedIndex = ((Picker)sender).SelectedIndex - 1;
                LogEntry("New Bt Device: " + ((Picker)sender).Items[selectedIndex + 1]);
            }
            ((Picker)sender).TextColor = Color.White;
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
        bool audioPlaying = false;
        int iter = 0;
        async void Button_Play_Clicked(object sender, EventArgs e)
        {
            AudioService service = new AudioService();
            if (!audioPlaying)
            {
                iter = 0;
                audioPlaying = true;
                service.PlayAudioFile("chills.wav");
                for (; iter < 100; iter++)
                {
                    await playButton.FadeTo(0.0, 1000, Easing.SinInOut);
                    await playButton.FadeTo(1.0, 1000, Easing.SinInOut);
                }
                service.CancelPlay();
                audioPlaying = false;
            }
            else
            {
                iter = 200;
            }
        }
        public bool SettingsTabOpen = false;
        private async void PlayButton_Clicked(object sender, EventArgs e)
        {
            if (Logger == null)
            {
                Logger = EntryLog;
            }
            if (!SettingsTabOpen)
                await SettingTab.TranslateTo(-270,0, 500, Easing.SinInOut);
            else
                await SettingTab.TranslateTo(0, 0.5, 500, Easing.SinInOut);

            SettingsTabOpen = !SettingsTabOpen;

        }

        
        public static void LogEntry(string Entry)
        {
            Device.BeginInvokeOnMainThread(() => { ActivePage.EditorMagic(Entry); });
        }

        string LogWait = "";
        private void EditorMagic(string Entry)
        {
            Android.Util.Log.Debug("friendly", Entry);
            if (Logger != null)
            {
                Logger.Text += Entry + "\n";
                if (LogWait != "")
                    Logger.Text += LogWait;
            }
            else
            {
                LogWait += (Entry + "\n");
                LogWait = "";
            }
        }


        private async void TacoButton_Clicked(object sender, EventArgs e)
        {
            for (int i = 0; i < 10; i++)
            {
                new Taco(HeadLayout);
                await Task.Delay(100);
            }
        }

        private void AutoButton_Clicked(object sender, EventArgs e)
        {

        }

        private void ManualButton_Clicked(object sender, EventArgs e)
        {

        }

        private void AccelerationSlider_DragCompleted(object sender, EventArgs e)
        {
            if (testService != null)
            {
                if (testService.gatt != null)
                {
                    testService.WriteGattCharacteristic((int)BTCodeOut.BeginAccelerationRange + (int)((Slider)sender).Value);
                }

            }
        }
    }




}

