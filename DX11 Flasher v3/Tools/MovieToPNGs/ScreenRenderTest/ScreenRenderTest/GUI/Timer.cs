using System;
using Microsoft.Xna.Framework;

namespace GUI
{
    public class Timer
    {
        protected float interval;
        protected float countDown;
        protected bool paused;
        protected bool timerTriggered;

        public Timer(float interval)
            : this(interval, interval)
        {
        }

        public Timer(float countDown, float interval)
        {
            this.countDown = countDown;
            this.interval = interval;
        }

        public void update(GameTime gameTime)
        {
            timerTriggered = false;
            if (!paused)
            {
                countDown -= gameTime.ElapsedGameTime.Milliseconds;

                if (countDown < 0)
                {
                    // TODO: Note there is technically a bug here where time is lost
                    // is only an issue if timer is used for repeated actions
                    timerTriggered = true;
                    countDown += interval;
                }
            }
        }

        public void setTime(float time)
        {
            this.countDown = time;
        }

        public int getTime()
        {
            return (int)countDown;
        }

        public bool wasTriggered()
        {
            return timerTriggered;
        }

        public void setInterval(float interval)
        {
            this.interval = interval;
        }

        public int getInterval()
        {
            return (int)interval;
        }

        public float getTimePercent()
        {
            return (interval - countDown) / interval; 
        }

        public void setPaused(bool paused)
        {
            this.paused = paused;
        }

        public bool getPaused()
        {
            return paused;
        }

        public double getTimeInSeconds()
        {
            return getTimeInSeconds(0);
        }

        public double getTimeInSeconds(int decimalPlaces)
        {
            double time = (int)(Math.Pow(10.0, decimalPlaces - 3) * countDown);
            return time * Math.Pow(10.0, -decimalPlaces);
        }

        public string getFormatedTime()
        {
            int fullTime = (int)getTimeInSeconds();
            int minutes = fullTime / 60;
            int seconds = fullTime % 60;

            string result = "";
            string modString = (seconds < 10) ? "0" : "";
            result = minutes + ":" + modString + seconds;
            return result;
        }
    }
}
