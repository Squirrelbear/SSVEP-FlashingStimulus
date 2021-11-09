using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI
{
    public class Panel : WndComponent
    {
        protected List<WndComponent> components;

        public Panel(Rectangle dest)
            : base(dest)
        {
            components = new List<WndComponent>();
        }

        public Panel(Rectangle dest, Texture2D background)
            : base(dest, background)
        {
            components = new List<WndComponent>();
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);

            foreach (WndComponent c in components)
            {
                c.update(gameTime);
            }
        }

        public override void draw(Microsoft.Xna.Framework.Graphics.SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);

            foreach(WndComponent c in components)
            {
                c.draw(spriteBatch);
            }
        }

        public override void mouseMoved(Point oldP, Point newP)
        {
            base.mouseMoved(oldP, newP);
            foreach (WndComponent c in components)
            {
                if(c.isVisible())
                    c.mouseMoved(oldP, newP);
            }
        }

        public override void mouseClicked(Point p)
        {
            base.mouseClicked(p);

            foreach (WndComponent c in components)
            {
                if (c.isVisible())
                    c.mouseClicked(p);
            }
        }

        public void addComponent(WndComponent component)
        {
            components.Add(component);
        }

        public List<WndComponent> getComponents()
        {
            return components;
        }

        public void removeComponent(WndComponent component)
        {
            components.Remove(component);
        }
    }
}
