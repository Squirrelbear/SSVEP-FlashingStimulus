using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace ScreenRenderTest
{
    public class RenderModeManager
    {
        public enum SplitMode
        {
            NoMode = 0, Grid1x1 = 1, Grid2x2 = 2, Grid4x4 = 3, Grid8x8 = 4, Grid16x16 = 5, Grid32x32 = 6, Grid64x64 = 7,
            Grid4x3 = 8, Grid5x4 = 9, Grid16x9 = 10, Grid16x10 = 11
        };

        protected SplitMode curSplitMode;
        protected RegionRender.CursorMode curCursorMode;
        protected RegionRender regionRender;
        protected GraphicsDevice device;
        protected GUI.InputManager inputManger;

        protected SpriteFont font;
        protected string screenModeStr, cursorModeStr, backgroundModeStr, flashModeStr, offsetTimeStr, coolLevelStr, blackTickStr;
        protected Vector2 posOne, posTwo, posThree, posFour, posFive, posSix, posSeven;
        protected int offsetTime, coolLevel;

        public RenderModeManager(GraphicsDevice device, SpriteFont font)
        {
            this.device = device;
            this.font = font;
            this.curSplitMode = SplitMode.Grid8x8;
            this.curCursorMode = RegionRender.CursorMode.Dot;
            inputManger = new GUI.InputManager(1);
            resetRegionRender();

            offsetTime = 0;
            coolLevel = 300;

            posOne = new Vector2(10, 10);
            posTwo = new Vector2(10, 100);
            posThree = new Vector2(10, 190);
            posFour = new Vector2(10, 235);
            posFive = new Vector2(10, 325);
            posSix = new Vector2(10, 415);
            posSeven = new Vector2(10, 505);
            screenModeStr = "Screen Mode (" + (int)(curSplitMode) + "): " + curSplitMode.ToString() 
                                + "\nPress Q to toggle through Screen modes.";
            cursorModeStr = "Cursor Mode (" + (int)(curCursorMode) + "): " + curCursorMode.ToString()
                                + "\nPress W to toggle through Cursor modes.";
            backgroundModeStr = "Background Mode Toggle: Press E";
            flashModeStr = "Black flash on Cursor (every 5 frames): Enabled(" + regionRender.isFlashBlack() + ")"
                    + "\nPress R to toggle black flashing.";
            offsetTimeStr = "Offset Cell Time: " + offsetTime + "ms\nPress T to toggle through times.";
            coolLevelStr = "Default frame time: " + coolLevel + "ms\nPress Y to toggle through times.";
            blackTickStr = "Black flash interval: " + regionRender.getBlackTickTime() + "\nPress U to toggle through times.";
        }

        public void resetRegionRender()
        {
            if(curSplitMode == SplitMode.NoMode)
                regionRender = null;

            int width = 1, height = 1;
            switch(curSplitMode)
            {
                case SplitMode.Grid1x1:
                    width = height = 1;
                    break;
                case SplitMode.Grid2x2:
                    width = height = 2;
                    break;
                case SplitMode.Grid4x4:
                    width = height = 4;
                    break;
                case SplitMode.Grid8x8:
                    width = height = 8;
                    break;
                case SplitMode.Grid16x16:
                    width = height = 16;
                    break;
                case SplitMode.Grid32x32:
                    width = height = 32;
                    break;
                case SplitMode.Grid64x64:
                    width = height = 64;
                    break;
                case SplitMode.Grid4x3:
                    width = 4;
                    height = 3;
                    break;
                case SplitMode.Grid5x4:
                    width = 5;
                    height = 4;
                    break;
                case SplitMode.Grid16x9:
                    width = 16;
                    height = 9;
                    break;
                case SplitMode.Grid16x10:
                    width = 16;
                    height = 10;
                    break;
            }

            regionRender = new RegionRender(device, new Vector2(width, height));
            regionRender.setCursorMode(curCursorMode);

            coolLevel = 300;
            offsetTime = 0;

            flashModeStr = "Black flash on Cursor (every 5 frames): Enabled(" + regionRender.isFlashBlack() + ")"
                    + "\nPress R to toggle black flashing.";
            coolLevelStr = "Default frame time: " + coolLevel + "ms\nPress Y to toggle through times.";
            offsetTimeStr = "Offset Cell Time: " + offsetTime + "ms\nPress T to toggle through times.";
            blackTickStr = "Black flash interval: " + regionRender.getBlackTickTime() + "\nPress U to toggle through times.";
        }

        public void update(GameTime gameTime)
        {
            inputManger.update(gameTime);

            if (regionRender != null)
            {
                MouseState mouseState = Mouse.GetState();
                Point mousePoint = new Point(mouseState.X, mouseState.Y);
                regionRender.update(gameTime, mousePoint);
            }

            if (inputManger.isKeyPressed(Keys.Q))
            {
                int tempScreenMode = (int)curSplitMode;
                tempScreenMode++;
                if (tempScreenMode > 11)
                    tempScreenMode = 0;
                curSplitMode = (SplitMode)tempScreenMode;
                screenModeStr = "Screen Mode (" + (int)(curSplitMode) + "): " + curSplitMode.ToString()
                                + "\nPress Q to toggle through Screen modes.";
                resetRegionRender();
            }
            else if (inputManger.isKeyPressed(Keys.W))
            {
                int tempCursorMode = (int)curCursorMode;
                tempCursorMode++;
                if (tempCursorMode > 5)
                    tempCursorMode = 0;
                curCursorMode = (RegionRender.CursorMode)tempCursorMode;
                regionRender.setCursorMode(curCursorMode);
                cursorModeStr = "Cursor Mode (" + (int)(curCursorMode) + "): " + curCursorMode.ToString()
                                    + "\nPress W to toggle through Cursor modes.";
            }
            else if (inputManger.isKeyPressed(Keys.R))
            {
                regionRender.setFlashBlack(!regionRender.isFlashBlack());
                flashModeStr = "Black flash on Cursor (every 5 frames): Enabled(" + regionRender.isFlashBlack() + ")"
                    + "\nPress R to toggle black flashing.";
            }
            else if (inputManger.isKeyPressed(Keys.T))
            {
                offsetTime += 5;
                if (offsetTime > 30)
                    offsetTime = 0;

                offsetTimeStr = "Offset Cell Time: " + offsetTime + "ms\nPress T to toggle through times.";
                regionRender.setOffsetTime(offsetTime);
            }
            else if (inputManger.isKeyPressed(Keys.Y))
            {
                coolLevel += 50;
                if (coolLevel > 300)
                    coolLevel = 50;

                coolLevelStr = "Default frame time: " + coolLevel + "ms\nPress Y to toggle through times.";
                regionRender.setCoolLevel(coolLevel);
            }
            else if (inputManger.isKeyPressed(Keys.U))
            {
                int nextTime = regionRender.getBlackTickTime() + 1;
                if (nextTime > 5)
                {
                    nextTime = 2;
                }
                regionRender.setBlackTickTime(nextTime);
                blackTickStr = "Black flash interval: " + regionRender.getBlackTickTime() + "\nPress U to toggle through times.";
            }
        }

        public void drawBegin()
        {
            if (regionRender != null)
            {
                regionRender.beginDrawToTexture();
            }
        }

        public void drawEnd(GameTime gameTime, SpriteBatch spriteBatch)
        {
            if (regionRender != null)
            {
                regionRender.endDrawToTexture();
                regionRender.draw(gameTime, spriteBatch);
            }

            spriteBatch.Begin();
            spriteBatch.DrawString(font, screenModeStr, posOne, Color.White);
            spriteBatch.DrawString(font, cursorModeStr, posTwo, Color.White);
            spriteBatch.DrawString(font, backgroundModeStr, posThree, Color.White);
            spriteBatch.DrawString(font, flashModeStr, posFour, Color.White);
            spriteBatch.DrawString(font, offsetTimeStr, posFive, Color.White);
            spriteBatch.DrawString(font, coolLevelStr, posSix, Color.White);
            spriteBatch.DrawString(font, blackTickStr, posSeven, Color.White);
            spriteBatch.End();
        }

        public GUI.InputManager getInputManager()
        {
            return inputManger;
        }
    }
}
