using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

using System.Diagnostics;

namespace ScreenRenderTest
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        public enum BackgroundMode { White = 0, Black = 1, Shade = 2, Video = 3 };

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        //RegionRender regionRender;
        //RenderModeManager renderMgr;
        Texture2D pixel;
        Rectangle displayRegion;
        float visibility;
        bool increase;

        Stopwatch stopwatch;
        MovieToPNG movietopng;

        // Video component
        Video video;
        VideoPlayer player;
        Texture2D videoTexture;
        //bool showVideo;

        FpsMonitor fps;
        SpriteFont font;
        string fpsStr;
        Vector2 fpsStrPos;

        GUI.InputManager inputManager;
        GUI.WndHandle optionWnd;

        BackgroundMode bgMode;
        Flasher flasher;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            graphics.PreferredBackBufferWidth = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
            graphics.PreferredBackBufferHeight = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;

            //SetFrameRate(graphics, 250);
            //graphics.SynchronizeWithVerticalRetrace = false;
            //IsFixedTimeStep = false;
            setVSync(true);
            //graphics.ToggleFullScreen();
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            //fps = new FpsMonitor();
            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            inputManager = new GUI.InputManager(1);
            /*video = Content.Load<Video>("QualitySample");
            player = new VideoPlayer();

            //int numberOfCellsSquared = 2; // eg. 20 = 20x20 grid of squares
            font = Content.Load<SpriteFont>("WndContent\\Fonts\\smallFont");
            //renderMgr = new RenderModeManager(GraphicsDevice, font);

            //regionRender = new RegionRender(GraphicsDevice, numberOfCellsSquared, font);
            int wndHeight = GraphicsDevice.Viewport.Height;
            int wndWidth = GraphicsDevice.Viewport.Width;
            this.displayRegion = new Rectangle(0, 0, wndWidth, wndHeight);
            visibility = 1;
            increase = false;
            pixel = Content.Load<Texture2D>("pixelcolour");

            this.IsMouseVisible = true;
            stopwatch = new Stopwatch();
            //showVideo = true;

            fpsStrPos = new Vector2(displayRegion.Right - 300, 10);

            inputManager = new GUI.InputManager(1);
            optionWnd = new OptionDialog(new Rectangle(10, 10, 375, 400), this);

            bgMode = BackgroundMode.Video;
            player.IsLooped = true;
            player.Play(video);

            flasher = new Flasher(graphics.GraphicsDevice, Content.Load<Texture2D>("white"), displayRegion);*/

            int wndHeight = GraphicsDevice.Viewport.Height;
            int wndWidth = GraphicsDevice.Viewport.Width;
            this.displayRegion = new Rectangle(0, 0, wndWidth, wndHeight);
            movietopng = new MovieToPNG(this.Content, GraphicsDevice, displayRegion, new Vector2(640, 360), 25, "C:\\outputVideo\\"); 
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            inputManager.update(gameTime);

            // Allows the game to exit
            if (inputManager.isBtnPressed(Buttons.Back, 1) || inputManager.isKeyPressed(Keys.Escape))
                this.Exit();

            movietopng.update(gameTime);
            if (movietopng.isFinished())
            {
                this.Exit();
            }

            /*if (inputManager.isKeyPressed(Keys.E))
                showVideo = !showVideo;*/

            /*if (bgMode == BackgroundMode.Shade)
            {
                if (increase)
                    visibility += 0.2f * gameTime.ElapsedGameTime.Milliseconds / 1000.0f;
                else
                    visibility -= 0.2f * gameTime.ElapsedGameTime.Milliseconds / 1000.0f;

                if (visibility > 1)
                {
                    visibility = 1;
                    increase = false;
                }
                else if (visibility < 0)
                {
                    visibility = 0;
                    increase = true;
                }
            }*/

            /*MouseState mouseState = Mouse.GetState();
            Point mousePoint = new Point(mouseState.X, mouseState.Y);
            regionRender.update(gameTime, mousePoint);*/

            //renderMgr.update(gameTime);

            /*if (player.State == MediaState.Stopped)
            {
                player.IsLooped = true;
                player.Play(video);
            }
            else if (bgMode != BackgroundMode.Video && player.State == MediaState.Playing)
            {
                player.Pause();
            }
            else if (bgMode == BackgroundMode.Video && player.State == MediaState.Paused)
            {
                player.Resume();
            }

            optionWnd.update(gameTime);

            fps.Update();
            fpsStr = "FPS: " + fps.Value + "\nFrame time: " + (1000 / fps.Value) + "ms";

            flasher.update(gameTime);
            */
            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            //regionRender.beginDrawToTexture();

            //renderMgr.drawBegin();
            
            /*if(bgMode == BackgroundMode.Video)
                drawVideo(spriteBatch);
            else if(bgMode == BackgroundMode.Shade)
                drawContent(spriteBatch);
            else if(bgMode == BackgroundMode.Black)
                GraphicsDevice.Clear(Color.Black);
            else if(bgMode == BackgroundMode.White)
                GraphicsDevice.Clear(Color.White);

            flasher.draw(spriteBatch, gameTime);

            spriteBatch.Begin();
            optionWnd.draw(spriteBatch);
            spriteBatch.End();
            //renderMgr.drawEnd(gameTime, spriteBatch);

            regionRender.endDrawToTexture();

            stopwatch.Reset();
            stopwatch.Start();
            regionRender.draw(gameTime, spriteBatch);
            stopwatch.Stop();
            Console.WriteLine("Ticks: " + stopwatch.ElapsedTicks + " mS: " + stopwatch.ElapsedMilliseconds);*/

            /*fps.frameOccurence();
            
            spriteBatch.Begin();
            Color c = (bgMode == BackgroundMode.White) ? Color.Black : Color.White;
            spriteBatch.DrawString(font, fpsStr, fpsStrPos, c);
            spriteBatch.End();*/

            movietopng.drawVideo(spriteBatch);

            base.Draw(gameTime);
        }

        private void drawContent(SpriteBatch spriteBatch)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin();
            spriteBatch.Draw(pixel, displayRegion, new Rectangle(0, 0, 1, 1), Color.White * visibility);
            spriteBatch.End();
        }

        private void drawVideo(SpriteBatch spriteBatch)
        {
            // Only call GetTexture if a video is playing or paused
            if (player.State != MediaState.Stopped)
                videoTexture = player.GetTexture();

            // Drawing to the rectangle will stretch the 
            // video to fill the screen
            Rectangle screen = new Rectangle(GraphicsDevice.Viewport.X,
                GraphicsDevice.Viewport.Y,
                GraphicsDevice.Viewport.Width,
                GraphicsDevice.Viewport.Height);

            // Draw the video, if we have a texture to draw.
            if (videoTexture != null)
            {
                spriteBatch.Begin();
                spriteBatch.Draw(videoTexture, screen, Color.White);
                spriteBatch.End();
            }
        }

        public GUI.InputManager getInputManager()
        {
            return inputManager;
        }

        public void setBGMode(BackgroundMode bgMode)
        {
            this.bgMode = bgMode;
        }

        public void setFlashHz(int hz)
        {
            flasher.setIntervalHz(hz);
        }

        public void setFlashColour(Color c)
        {
            flasher.setFlashColour(c);
        }

        public void setFlashMode(Flasher.FlashMode flashMode)
        {
            flasher.setFlashMode(flashMode);
        }

        public void setVSync(bool enabled)
        {
            graphics.SynchronizeWithVerticalRetrace = true;
            graphics.ApplyChanges();
        }

        public void setGridHeight(int height)
        {
            flasher.setGridHeight(height);
        }

        public void setGridWidth(int width)
        {
            flasher.setGridWidth(width);
        }

        public void SetFrameRate(int frames)
        {
            SetFrameRate(graphics, frames);
        }

        public void SetFrameRate(GraphicsDeviceManager manager, int frames)
        {
            double dt = (double)1000 / (double)frames;
            manager.SynchronizeWithVerticalRetrace = false;
            TargetElapsedTime = TimeSpan.FromMilliseconds(dt);
            manager.ApplyChanges();
        }
    }
}
