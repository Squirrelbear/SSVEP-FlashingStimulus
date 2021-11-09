using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ScreenRenderTest
{
    public class RenderCell
    {
        // standard refresh interval 
        public static int DEFAULT_COOLLEVEL = 300;
        // max refresh interval
        public static int DEFAULT_HEAT = 10;
        // percent reduction in refresh rate per second when cooling
        public static float DEFAULT_COOLPERSECOND = 0.3f;
        // number of ticks between black swap out
        public static int DEFAULT_BLACKTICK = 5;

        // The render region for the specific cell
        protected Rectangle rect;
        // The time between refreshing the screen
        protected float interval;
        // The current clock status
        protected float status;
        // The default interval speed the region uses when not excited
        protected float coolLevel;
        // The offset from the standard clock that the cell uses
        protected float timeoffset;
        // The black tick controls the number of intervals between switching to a black frame
        protected int blackTick;
        // Enables the flashBlack property
        protected bool flashBlack;

        SpriteFont font;
        int updateNumber;
        RegionRender parentRender;

        public RenderCell(Rectangle rect, int interval, int coolLevel, int timeoffset, RegionRender parentRender)
            : this(rect, interval, coolLevel, parentRender)
        {
            this.timeoffset = timeoffset;
        }

        public RenderCell(Rectangle rect, int interval, int coolLevel, RegionRender parentRender)
        {
            this.rect = rect;
            this.interval = interval;
            this.coolLevel = coolLevel;
            status = interval + 1;
            this.parentRender = parentRender;
            timeoffset = 0;
            flashBlack = true;
        }

        public RenderCell(Rectangle rect, int interval, SpriteFont font, RegionRender parentRender)
            : this(rect, interval, interval, parentRender)
        {
            this.font = font;
            updateNumber = 0;
        }

        public void update(GameTime gameTime)
        {
            if (coolLevel == interval)
            {
                status = parentRender.getClock() + timeoffset;
                return;
            }
            else
            {
                interval += DEFAULT_COOLPERSECOND * coolLevel * gameTime.ElapsedGameTime.Milliseconds / 1000.0f;
                if (interval > coolLevel)
                {
                    status = parentRender.getClock() - timeoffset;
                    interval = coolLevel;
                }
            }
        }

        /*
        public void update(GameTime gameTime, Point mouse, int heat)
        {
            if (rect.Contains(mouse))
            {
                interval = heat;
            }
            else if (coolLevel == interval)
            {
                status = parentRender.getClock() - timeoffset;
                return;
            }
            else
            {
                interval += 0.3f * coolLevel * gameTime.ElapsedGameTime.Milliseconds / 1000.0f;
                if (interval > coolLevel)
                    interval = coolLevel;
            }
        }*/

        public void drawUpdate(GameTime gameTime, RenderTarget2D newRender, bool forceRenderCell, SpriteBatch spriteBatch)
        {
            status += gameTime.ElapsedGameTime.Milliseconds;

            if (status > interval || forceRenderCell)
            {
                status = status % interval;

                if (interval == DEFAULT_HEAT)
                {
                    blackTick--;
                    if (blackTick == -1)
                    {
                        blackTick = parentRender.getBlackTickTime();//DEFAULT_BLACKTICK;
                    }
                }
                else
                {
                    blackTick = -1;
                }

                if(blackTick == 0 && flashBlack)
                    spriteBatch.Draw(newRender, rect, rect, Color.Black);
                else
                    spriteBatch.Draw(newRender, rect, rect, Color.White);

                if(font != null)
                {
                    updateNumber++;
                    spriteBatch.DrawString(font, ""+updateNumber, new Vector2(rect.Center.X, rect.Center.Y), Color.White);
                }
            }
        }

        public void applyHeat(int heat)
        {
            interval = heat;
            if(blackTick == -1 && flashBlack)
                blackTick = parentRender.getBlackTickTime();//DEFAULT_BLACKTICK;
        }

        public void setFont(SpriteFont font)
        {
            this.font = font;
        }

        public void setFlashBlack(bool flashBlack)
        {
            this.flashBlack = flashBlack;
        }

        public void setBlackTick(int tick)
        {
            this.blackTick = tick;
        }

        public int getBlackTick()
        {
            return blackTick;
        }

        public void setTimeOffset(int offset)
        {
            this.timeoffset = offset;
        }

        public void setCoolLevel(int coolLevel)
        {
            this.coolLevel = coolLevel;
            this.interval = coolLevel;
        }
    }
}
