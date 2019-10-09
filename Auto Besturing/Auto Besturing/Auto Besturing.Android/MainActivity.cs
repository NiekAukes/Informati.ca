using System;

using Android;
using Android.App;
using Android.Content.PM;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;

namespace Auto_Besturing.Droid
{

    [Activity(Label = "Auto_Besturing", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation)]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        public static Xamarin.Forms.Application appl;
        public static AlertDialog.Builder alert;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            TabLayoutResource = Resource.Layout.Tabbar;
            ToolbarResource = Resource.Layout.Toolbar;

            base.OnCreate(savedInstanceState);

            Xamarin.Essentials.Platform.Init(this, savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);
            RequestPermissions(new string[] { Manifest.Permission.Bluetooth, Manifest.Permission.BluetoothAdmin, Manifest.Permission.BluetoothPrivileged, Manifest.Permission.AccessCoarseLocation, Manifest.Permission.AccessFineLocation }, 1);


            appl = new App();
            LoadApplication(application: appl);
            

            alert = new AlertDialog.Builder(this);
            alert.SetTitle("Specify Action");
            alert.SetMessage("Do you want to add or substract?");
            alert.SetPositiveButton("Ok", (senderAlert, args) => { });

        }
        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Xamarin.Essentials.Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);

            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }
        public static void CloseApplication()
        {
            System.Diagnostics.Process.GetCurrentProcess().CloseMainWindow();
        }
        public static void CloseApplication(string title, string message, string cancel, Android.Bluetooth.BluetoothAdapter adapter)
        {
            if (appl != null)
            {
                if (adapter.IsDiscovering)
                    adapter.CancelDiscovery();
                appl.MainPage.DisplayAlert(title, message, cancel);
                System.Diagnostics.Process.GetCurrentProcess().CloseMainWindow();
            }
        }
    }


}