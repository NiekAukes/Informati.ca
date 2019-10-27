using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Threading;

using Auto_Besturing.Droid;
using AudioPlayEx.Droid;

using Xamarin.Forms;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Media;
using Android.Content.Res;

[assembly: Dependency(typeof(AudioService))]
namespace AudioPlayEx.Droid
{
    public class AudioService : IAudio
    {
        public MediaPlayer player = new MediaPlayer();
        string file;
        public AudioService()
        {
        }

        public void PlayAudioFile(string fileName)
        {
            file = fileName;
            var fd = global::Android.App.Application.Context.Assets.OpenFd(fileName);
            player.Prepared += (s, e) =>
            {
                player.Start();
            };
            player.SetDataSource(fd.FileDescriptor, fd.StartOffset, fd.Length);
            player.Prepare();
        }
        public void CancelPlay()
        {
            
            player.Reset();
        }
        public void Pause()
        {
            if (!player.IsPlaying)
            {
                throw new InvalidOperationException("Cannot Cancel if not playing");
            }
            player.Pause();
        }
        public void Resume()
        {
            if (player.IsPlaying)
            {
                if (file == "")
                {
                    throw new InvalidOperationException("Cannot Cancel if not playing");
                }
                else PlayAudioFile(file);
            }
            player.Prepare();
        }
    }
}

namespace AudioPlayEx
{
    public interface IAudio
    {
        void PlayAudioFile(string fileName);
    }
}
