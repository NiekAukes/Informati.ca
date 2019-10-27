using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;

namespace Auto_Besturing.Droid
{
    public class ErrorHandler
    {
        public ErrorHandler(string message)
        {
            Android.Util.Log.Error("Friendly", message);
        }
        public ErrorHandler(string message, Exception innerException)
        {
            Android.Util.Log.Error("Exception", message);
            throw innerException;
        }
        public ErrorHandler(string message, Xamarin.Forms.Page page, Exception innerException)
        {
            Android.Util.Log.Error("Exception", message);
            page.DisplayAlert("An Error Occured", message, "Ok");
            throw innerException;
        }
        public ErrorHandler(string message, Xamarin.Forms.Page page)
        {
            Android.Util.Log.Error("Exception", message);
            page.DisplayAlert("An Error Occured", message, "Ok");
        }
    }
}