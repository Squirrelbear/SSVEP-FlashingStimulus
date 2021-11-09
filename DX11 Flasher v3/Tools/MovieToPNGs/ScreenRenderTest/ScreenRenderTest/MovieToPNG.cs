using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework;
using System.IO;

namespace ScreenRenderTest
{
    public class MovieToPNG
    {
        Video video;
        VideoPlayer player;
        Texture2D videoTexture;
        GraphicsDevice device;

        protected RenderTarget2D renderTarget;

        Rectangle displayRect;
        Rectangle outputRect;
        int fpsCapture;
        string outputDir;
        GUI.Timer timer;

        bool finished;
        bool renderFrame;

        List<Texture2D> images;

        public MovieToPNG(ContentManager Content, GraphicsDevice device, Rectangle displayRect, Vector2 outputDims, int fpsCapture, string outputDir)
        {
            this.device = device;
            this.displayRect = displayRect;
            this.outputRect = new Rectangle(0, 0, (int)outputDims.X, (int)outputDims.Y);
            this.fpsCapture = fpsCapture;
            this.outputDir = outputDir;
            finished = false;
            images = new List<Texture2D>();

            renderTarget = new RenderTarget2D(
                device,
                outputRect.Width,
                outputRect.Height,
                false,
                device.PresentationParameters.BackBufferFormat,
                DepthFormat.Depth24);

            int frameSplitTime = 1000 / fpsCapture;
            timer = new GUI.Timer(frameSplitTime);

            video = Content.Load<Video>("QualitySample");
            player = new VideoPlayer();
            player.Play(video);
        }

        public void update(GameTime gameTime)
        {
            if (!finished)
            {
                if (player.State == MediaState.Stopped)// || images.Count >= 400)
                {
                    int count = images.Count;
                    for (int i = 0; i < count; i++)
                    {
                        using(FileStream fs = new FileStream(outputDir + @"video"+i+@".png", FileMode.OpenOrCreate))
                        {
                            Console.WriteLine("Saving Image: " + i);
                            images[0].SaveAsPng(fs, images[0].Width, images[0].Height);
                            images[0].Dispose();
                            images.RemoveAt(0);
                        }
                    }

                    finished = true;
                }

                timer.update(gameTime);
                if (timer.wasTriggered())
                {
                    renderFrame = true;
                }
            }
        }

        public bool isFinished()
        {
            return finished;
        }

        public void drawVideo(SpriteBatch spriteBatch)
        {
            if (!finished)
            {
                // Only call GetTexture if a video is playing or paused
                if (player.State != MediaState.Stopped)
                    videoTexture = player.GetTexture();

                // Draw the video, if we have a texture to draw.
                if (videoTexture != null)
                {
                    device.SetRenderTarget(renderTarget);
                    device.DepthStencilState = new DepthStencilState() { DepthBufferEnable = true };

                    spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend,
                                SamplerState.LinearClamp, DepthStencilState.Default,
                                RasterizerState.CullNone);
                    spriteBatch.Draw(videoTexture, outputRect, Color.White);
                    
                    spriteBatch.End();

                    device.SetRenderTarget(null);

                    Texture2D capture = new Texture2D(device, renderTarget.Width, renderTarget.Height);
                    Color[] bits = new Color[renderTarget.Width * renderTarget.Height];
                    renderTarget.GetData(bits);
                    capture.SetData(bits);
                    images.Add(capture);

                    device.Clear(Color.Black);

                    spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend,
                                SamplerState.LinearClamp, DepthStencilState.Default,
                                RasterizerState.CullNone);

                    spriteBatch.Draw(videoTexture, displayRect, Color.White);

                    spriteBatch.End();
                }
                else if (renderFrame)
                {
                    Console.WriteLine("Error... Frame that was meant to be saved could not be generated.");
                }

                renderFrame = false;
            }
        }


    }
}
