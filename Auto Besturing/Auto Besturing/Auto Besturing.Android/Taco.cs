using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;

using Xamarin.Forms;

namespace Auto_Besturing.Droid
{
    class Taco
    {
        int pos = 0;
        ImageButton image;
        Random random = new Random();
        AbsoluteLayout layout;
        bool TacoFrenzyActive = true;
        public Taco(AbsoluteLayout lay)
        {
            layout = lay;
            image = new ImageButton()
            {
                Source = "Taco.png",
                BackgroundColor = Color.Transparent
            };
            layout.Children.Add(image);
            pos = layout.Children.Count - 1;
            Xamarin.Forms.AbsoluteLayout.SetLayoutFlags(image, AbsoluteLayoutFlags.PositionProportional);
            Xamarin.Forms.AbsoluteLayout.SetLayoutBounds(image, new Rectangle(0.5, 0.5, 100, 100));
            Android.Util.Log.Debug("friendly", "Made Taco");
            image.Clicked += (s, e) => { layout.Children[pos] = new Image(); TacoFrenzyActive = false; }; 
            StartDing();

        }
        public async void StartDing()
        {
            await Task.Delay(1000);
            await ActivateTacoModeAsync();
        }
        public async Task ActivateTacoModeAsync()
        {
            int PreviousPixelX = 0;
            int PreviousPixelY = 0;
            for (int i = 0; i < 100 && TacoFrenzyActive; i++)
            {
                int PixelX = random.Next(-300, 300);
                int PixelY = random.Next(-180, 180);

                int DeltaX = PreviousPixelX - PixelX;
                int DeltaY = PreviousPixelY - PixelY;

                double Distance = Math.Sqrt(Math.Pow(DeltaX, 2) + Math.Pow(DeltaY, 2));
                await image.TranslateTo(PixelX, PixelY, ((uint)Distance * 2), Easing.Linear);

                PreviousPixelX = PixelX;
                PreviousPixelY = PixelY;
            }
            layout.Children[pos] = new Image();
            TacoFrenzyActive = false;
        }
    }
}