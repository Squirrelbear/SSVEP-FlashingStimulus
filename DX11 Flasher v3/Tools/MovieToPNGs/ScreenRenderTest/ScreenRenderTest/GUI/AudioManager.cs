using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace GUI
{
    public class AudioManager
    {
        public enum MusicMode { None, Sequence, Random, PlayOnce, Repeat };
        
        // music variables
        private List<Song> music;
        private int curSongID;
        private Random gen;
        private MusicMode musicMode;

        private ScreenRenderTest.Game1 appRef;
        private InputManager inputManager;

        public AudioManager(List<string> songPaths, ScreenRenderTest.Game1 appRef)
            : this(songPaths, MusicMode.Sequence, -1, appRef)
        {
        }

        public AudioManager(List<string> songPaths, MusicMode mode, ScreenRenderTest.Game1 appRef)
            : this(songPaths, mode, -1, appRef)
        {
        }

        public AudioManager(List<string> songPaths, MusicMode mode, int startSongID, ScreenRenderTest.Game1 appRef)
        {
            this.appRef = appRef;
            this.inputManager = appRef.getInputManager();
            music = new List<Song>();

            foreach (string file in songPaths)
            {
                music.Add(loadSong(file));
            }

            curSongID = startSongID;
            this.musicMode = mode;

            gen = new Random();
            if (mode == MusicMode.Random)
            {
                curSongID = gen.Next(music.Count);
            }
        }

        public void update(GameTime gameTime)
        {
            if (MediaPlayer.State == MediaState.Stopped)
            {
                startNextSong();
            }
            else if (MediaPlayer.State == MediaState.Playing && inputManager.isKeyPressed(Keys.M))
            {
                MediaPlayer.Pause();
            }
            else if (MediaPlayer.State == MediaState.Paused && inputManager.isKeyPressed(Keys.M))
            {
                MediaPlayer.Resume();
            }

            if (inputManager.isKeyPressed(Keys.OemMinus))
            {
                if (MediaPlayer.Volume >= 0.0)
                    MediaPlayer.Volume -= 0.05f;
            }
            else if (inputManager.isKeyPressed(Keys.OemPlus))
            {
                if (MediaPlayer.Volume <= 1.0)
                    MediaPlayer.Volume += 0.05f;
            }
        }

        public void mute(bool ismute)
        {
            if (ismute)
            {
                MediaPlayer.Pause();
            }
            else
            {
                MediaPlayer.Resume();
            }
        }

        public void startNextSong()
        {
            if(musicMode == MusicMode.Random)
            {
                // fix for when there is only one song
                if (music.Count == 1)
                {
                    setTrackAndPlay(0);
                    return;
                }
                    

                int nextSongID;
                do
                {
                    nextSongID = gen.Next(music.Count);
                } while (nextSongID == curSongID);
                setTrackAndPlay(nextSongID);
            }
            else if (musicMode == MusicMode.Sequence)
            {
                curSongID++;
                if (curSongID >= music.Count)
                    curSongID = 0;
                setTrackAndPlay(curSongID);
            }
            else if (musicMode == MusicMode.Repeat)
            {
                setTrackAndPlay(curSongID);
            }
        }

        public void setMode(MusicMode mode)
        {
            this.musicMode = mode;
        }

        public void setTrackAndPlay(int trackID)
        {
            if (trackID < 0 || trackID >= music.Count)
                return;

            bool needMute = MediaPlayer.State == MediaState.Paused;
            curSongID = trackID;
            MediaPlayer.Stop();
            MediaPlayer.Play(music[curSongID]);
            mute(needMute);
        }

        public int getCurSongID()
        {
            return curSongID;
        }

        public SoundEffect loadSound(string file)
        {
            return appRef.Content.Load<SoundEffect>(file);
        }

        public Song loadSong(string file)
        {
            return appRef.Content.Load<Song>(file);
        }
    }
}
