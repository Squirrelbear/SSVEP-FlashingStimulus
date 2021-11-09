using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Audio;

namespace GUI
{
    public class WndHandle
    {
        public enum WndType { MainMenu, Credits, Help, LevelViewer, Options, PlayConfig, PostGame, ExitGame, Overlay };
        protected WndType wndType;
        protected ScreenRenderTest.Game1 appRef;
        protected InputManager inputManager;
        //protected AudioManager audioManager;
        protected Rectangle displayRect;
        protected bool isVisible;
        protected List<WndComponent> components;

        public WndHandle(WndType wndType, Rectangle displayRect, ScreenRenderTest.Game1 appRef)
        {
            this.wndType = wndType;
            this.appRef = appRef;
            this.displayRect = displayRect;
            this.inputManager = appRef.getInputManager();
            //this.audioManager = appRef.getAudioManager();
            isVisible = true;
            components = new List<WndComponent>();
        }

        public virtual void update(GameTime gameTime)
        {
            bool mouseMoved = inputManager.hasMouseMoved();
            bool mouseClicked = inputManager.getMouseClicked();

            foreach (WndComponent c in components)
            {
                c.update(gameTime);
            }

            if (mouseMoved)
            {
                foreach (WndComponent c in components)
                {
                    if(c.isVisible())
                        c.mouseMoved(inputManager.getLastCursor(), inputManager.getCursor());
                }
            }

            if (mouseClicked)
            {
                foreach (WndComponent c in components)
                {
                    if (c.isVisible())
                        c.mouseClicked(inputManager.getCursor());
                }
            }
        }

        public virtual void draw(SpriteBatch spriteBatch)
        {
            foreach (WndComponent c in components)
            {
                if (c.isVisible())
                    c.draw(spriteBatch);
            }
        }

        public WndType getWndType()
        {
            return wndType;
        }

        public ScreenRenderTest.Game1 getAppRef()
        {
            return appRef;
        }

        public void setVisible(bool isVisible)
        {
            this.isVisible = isVisible;
        }

        public bool getVisible()
        {
            return isVisible;
        }

        public void addComponent(WndComponent component)
        {
            components.Add(component);
        }

        public Texture2D loadTexture(string file)
        {
            return appRef.Content.Load<Texture2D>(file);
        }

        public SpriteFont loadFont(string file)
        {
            return appRef.Content.Load<SpriteFont>("WndContent\\Fonts\\" + file);
        }

        public SoundEffect loadSound(string file)
        {
            return appRef.Content.Load<SoundEffect>(file);
        }
    }
}
