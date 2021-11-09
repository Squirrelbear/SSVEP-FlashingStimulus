using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace ScreenRenderTest
{
    public class Flasher
    {
        public enum FlashMode { Clear = 0, Sprite = 1, Grid = 2 };

        GUI.Timer timer;
        Color c;
        GraphicsDevice device;
        bool flash;
        FlashMode flashMode;
        Texture2D texture;
        Rectangle displayRect;
        Rectangle dest;
        Rectangle source;
        int gridWidth, gridHeight;
        int cellWidth, cellHeight;
        //Rectangle[,] grid;

        public Flasher(GraphicsDevice device, Texture2D texture, Rectangle displayRect)
        {
            c = Color.Black;
            timer = new GUI.Timer(HztoMilliseconds(1));
            this.device = device;
            flash = false;

            this.texture = texture;
            this.displayRect = displayRect;
            this.dest = displayRect;
            this.source = new Rectangle(0, 0, texture.Width, texture.Height);

            gridWidth = gridHeight = 8;
            //grid = new Rectangle[gridWidth, gridHeight];

            flashMode = FlashMode.Clear;
        }

        public void update(GameTime gameTime)
        {
            if (flashMode == FlashMode.Grid)
            {
                MouseState mouse = Mouse.GetState();
                dest = new Rectangle((mouse.X / cellWidth) * cellWidth, (mouse.Y / cellHeight) * cellHeight, cellWidth, cellHeight); 

                /*for (int i = 0; i < gridWidth; i++)
                {
                    for (int j = 0; j < gridHeight; j++)
                    {
                        if (grid[i, j].Contains(mousePoint))
                        {
                            dest = grid[i, j];
                            i = gridWidth;
                            break;
                        }
                    }
                }*/
            }

            //timer.update(gameTime);
            //flash = timer.wasTriggered();
        }

        public void draw(SpriteBatch spriteBatch, GameTime gameTime)
        {
            timer.update(gameTime);
            flash = timer.wasTriggered();

            if(flash)
            {
                if(flashMode == FlashMode.Clear)
                {
                    device.Clear(c);
                }
                else
                {
                    spriteBatch.Begin();
                    spriteBatch.Draw(texture, dest, source, c);
                    spriteBatch.End();
                }
            }
        }

        public void setIntervalHz(int Hz)
        {
            timer.setInterval(HztoMilliseconds(Hz));
        }

        public void setFlashColour(Color c)
        {
            this.c = c;
        }

        public float HztoMilliseconds(int Hz)
        {
            return 1000.0f / Hz;
        }

        public FlashMode getFlashMode()
        {
            return flashMode;
        }

        public void setFlashMode(FlashMode flashMode)
        {
            this.flashMode = flashMode;

            if (flashMode == FlashMode.Grid)
            {
                cellWidth = displayRect.Width / gridWidth;
                cellHeight = displayRect.Height / gridHeight;
                MouseState mouse = Mouse.GetState();
                dest = new Rectangle(mouse.X / cellWidth, mouse.Y / cellHeight, cellWidth, cellHeight); 

                /*grid = new Rectangle[gridWidth, gridHeight];
                LayoutManger lM = new LayoutManger(displayRect, gridHeight, gridWidth);
                for (int i = 0; i < gridWidth; i++)
                {
                    for (int j = 0; j < gridHeight; j++)
                    {
                        grid[i, j] = lM.nextRect();
                    }
                }*/
            }
            else
            {
                dest = displayRect;
            }
        }

        public void setGridHeight(int height)
        {
            gridHeight = height;
            cellHeight = displayRect.Height / gridHeight;
        }

        public void setGridWidth(int width)
        {
            gridWidth = width;
            cellWidth = displayRect.Width / gridWidth;
        }
    }
}
