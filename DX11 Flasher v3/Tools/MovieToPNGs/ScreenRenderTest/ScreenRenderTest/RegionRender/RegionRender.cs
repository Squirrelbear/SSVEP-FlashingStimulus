using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace ScreenRenderTest
{
    public class RegionRender
    {
        public enum CursorMode { NoMode = 0, Dot = 1, Grid3x3 = 2, Grid5x5 = 3, Grid7x7 = 4, Grid9x9 = 5 };

        protected Texture2D currentBuffer;
        protected RenderTarget2D renderTarget;
        protected RenderTarget2D outputRender;
        protected GraphicsDevice device;
        protected Rectangle displayRegion;
        protected RenderCell[,] regions;

        protected Vector2 cellDimensions;
        protected CursorMode curCursorMode;
        protected int clock;
        protected int globalInterval;

        protected bool forceRenderFrame;
        protected bool flashBlack;
        protected int blackTickTime;

        public RegionRender(GraphicsDevice device, int dimenions, SpriteFont font)
            : this(device, new Vector2(dimenions, dimenions), font)
        {
            
        }

        public RegionRender(GraphicsDevice device, int dimenions)
            : this(device, new Vector2(dimenions, dimenions), null)
        {
        }

        public RegionRender(GraphicsDevice device, Vector2 dimenions)
            : this(device, dimenions, null)
        {
        }

        public RegionRender(GraphicsDevice device, Vector2 dimenions, SpriteFont font)
        {
            this.device = device;
            int wndHeight = device.Viewport.Height;
            int wndWidth = device.Viewport.Width;
            this.displayRegion = new Rectangle(0, 0, wndWidth, wndHeight);

            renderTarget = new RenderTarget2D(
                device,
                device.PresentationParameters.BackBufferWidth,
                device.PresentationParameters.BackBufferHeight,
                false,
                device.PresentationParameters.BackBufferFormat,
                DepthFormat.Depth24);

            outputRender = new RenderTarget2D(
                device,
                device.PresentationParameters.BackBufferWidth,
                device.PresentationParameters.BackBufferHeight,
                false,
                device.PresentationParameters.BackBufferFormat,
                DepthFormat.Depth24);

            globalInterval = RenderCell.DEFAULT_COOLLEVEL;


            LayoutManger lM = new LayoutManger(displayRegion, (int)dimenions.X, (int)dimenions.Y);
            regions = new RenderCell[(int)dimenions.X, (int)dimenions.Y];
            for (int i = 0; i < regions.GetLength(0); i++)
            {
                for (int j = 0; j < regions.GetLength(1); j++)
                {
                    if (font == null)
                        regions[i, j] = new RenderCell(lM.nextRect(), globalInterval, globalInterval, this);
                    else
                        regions[i, j] = new RenderCell(lM.nextRect(), 500, font, this);

                    /*if(font == null)
                        regions[i, j] = new RenderCell(lM.nextRect(), (i * regions.GetLength(0) + j + 1) * 10);
                    else
                        regions[i, j] = new RenderCell(lM.nextRect(), (i * regions.GetLength(0) + j + 1) * 10, font);*/
                }
            }

            Rectangle dimensionRect = lM.getRect(0, 0);
            cellDimensions = new Vector2(dimensionRect.Width, dimensionRect.Height);
            curCursorMode = CursorMode.Dot;

            forceRenderFrame = true;
            flashBlack = true;
            blackTickTime = RenderCell.DEFAULT_BLACKTICK;
        }

        public void update(GameTime gameTime, Point pointer)
        {
            clock += gameTime.ElapsedGameTime.Milliseconds;

            if (clock > globalInterval)
            {
                clock = clock % globalInterval;
            }

            for (int i = 0; i < regions.GetLength(0); i++)
            {
                for (int j = 0; j < regions.GetLength(1); j++)
                {
                    regions[i, j].update(gameTime);
                }
            }

            if (curCursorMode == CursorMode.NoMode)
                return;

            int targetCellX = (int)(pointer.X / cellDimensions.X);
            int targetCellY = (int)(pointer.Y / cellDimensions.Y);

            int beginCellX = targetCellX;
            int beginCellY = targetCellY;
            int expand = 1;

            switch (curCursorMode)
            {
                case CursorMode.Dot:
                    break;
                case CursorMode.Grid3x3:
                    beginCellX--;
                    beginCellY--;
                    expand = 3;
                    break;
                case CursorMode.Grid5x5:
                    beginCellX-=2;
                    beginCellY-=2;
                    expand = 5;
                    break;
                case CursorMode.Grid7x7:
                    beginCellX-=3;
                    beginCellY-=3;
                    expand = 7;
                    break;
                case CursorMode.Grid9x9:
                    beginCellX-=4;
                    beginCellY-=4;
                    expand = 9;
                    break;
            }

            int blackTick = getBlackTickTime(); //RenderCell.DEFAULT_BLACKTICK;
            if (targetCellY < regions.GetLength(0) && targetCellY >= 0 && targetCellX < regions.GetLength(1) && targetCellX >= 0)
            {
                regions[targetCellY, targetCellX].applyHeat(RenderCell.DEFAULT_HEAT);
                // synchronise the black for group regions
                blackTick = regions[targetCellY, targetCellX].getBlackTick();
            }

            for (int x = beginCellX; x < beginCellX + expand; x++)
            {
                for (int y = beginCellY; y < beginCellY + expand; y++)
                {
                    if (x < 0 || y < 0 || x >= regions.GetLength(0) || y >= regions.GetLength(1))
                        return;

                    regions[y, x].applyHeat(RenderCell.DEFAULT_HEAT);
                    regions[y, x].setBlackTick(blackTick);
                }
            }
        }

        public void draw(GameTime gameTime, SpriteBatch spriteBatch)
        {
            Texture2D old = new Texture2D(device, outputRender.Width, outputRender.Height);
            Color[] bits = new Color[outputRender.Width * outputRender.Height];
            outputRender.GetData(bits);
            old.SetData(bits);

            device.SetRenderTarget(outputRender);
            device.DepthStencilState = new DepthStencilState() { DepthBufferEnable = true };

            spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend,
                        SamplerState.LinearClamp, DepthStencilState.Default,
                        RasterizerState.CullNone);
            spriteBatch.Draw(old, displayRegion, Color.White);
            for (int i = 0; i < regions.GetLength(0); i++)
            {
                for (int j = 0; j < regions.GetLength(1); j++)
                {
                    regions[i, j].drawUpdate(gameTime, renderTarget, forceRenderFrame, spriteBatch);
                }
            }

            spriteBatch.End();
            device.SetRenderTarget(null);

            device.Clear(Color.Black);

            spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend,
                        SamplerState.LinearClamp, DepthStencilState.Default,
                        RasterizerState.CullNone);

            spriteBatch.Draw(outputRender, displayRegion, Color.White);

            spriteBatch.End();

            forceRenderFrame = false;
        }

        public RenderTarget2D getRenderTarget()
        {
            return renderTarget;
        }

        public RenderTarget2D getOutputRender()
        {
            return outputRender;
        }

        public void beginDrawToTexture()
        {
            device.SetRenderTarget(renderTarget);
            device.DepthStencilState = new DepthStencilState() { DepthBufferEnable = true };
        }

        public void endDrawToTexture()
        {
            device.SetRenderTarget(null);
        }

        public void setCursorMode(CursorMode newCursorMode)
        {
            this.curCursorMode = newCursorMode;
        }

        public CursorMode getCursorMode()
        {
            return curCursorMode;
        }

        public float getClock()
        {
            return clock;
        }

        public void forceRenderNextFrame()
        {
            forceRenderFrame = true;
        }

        public bool isFlashBlack()
        {
            return flashBlack;
        }

        public void setFlashBlack(bool flashBlack)
        {
            this.flashBlack = flashBlack;

            for (int i = 0; i < regions.GetLength(0); i++)
            {
                for (int j = 0; j < regions.GetLength(1); j++)
                {
                    regions[i, j].setFlashBlack(flashBlack);
                }
            }
        }

        public void setOffsetTime(int increment)
        {
            int curAmount = increment;
            for (int i = 0; i < regions.GetLength(1); i++)
            {
                regions[regions.GetLength(0) - 1, i].setTimeOffset(curAmount);
                curAmount += increment;
            }

            for (int i = 0; i < regions.GetLength(0)-1; i++)
            {
                regions[i, regions.GetLength(1) - 1].setTimeOffset(curAmount);
                curAmount += increment;
            }
        }

        public void setCoolLevel(int coolLevel)
        {
            globalInterval = coolLevel;

            for (int i = 0; i < regions.GetLength(0); i++)
            {
                for (int j = 0; j < regions.GetLength(1); j++)
                {
                    regions[i, j].setCoolLevel(coolLevel);
                }
            }
        }

        public void setBlackTickTime(int time)
        {
            blackTickTime = time;
        }

        public int getBlackTickTime()
        {
            return blackTickTime;
        }
    }
}
