using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace GUI
{
    public class WndComponent
    {
        protected Vector2 location, objectSize;
        protected Rectangle dest;
        protected Texture2D texture;
        protected bool visible;
        protected bool hasFocus;
        protected bool bgTextureSet;

        public WndComponent(Rectangle dest)
            : this(dest, null)
        {
            visible = true;
            bgTextureSet = false;
        }

        public WndComponent(Rectangle dest, Texture2D texture)
        {
            setRect(dest);
            this.texture = texture;
            visible = true;
            hasFocus = false;
            bgTextureSet = true;
        }

        public virtual void update(GameTime gameTime)
        {

        }

        public virtual void draw(SpriteBatch spriteBatch)
        {
            if (visible && bgTextureSet)
            {
                //, new Rectangle(0,0,texture.Width, texture.Height)
                spriteBatch.Draw(texture, dest, Color.White);
            }
        }

        public virtual void setRect(Rectangle dest)
        {
            Rectangle newDest = new Rectangle();
            newDest.X = dest.X;
            newDest.Y = dest.Y;
            newDest.Width = dest.Width;
            newDest.Height = dest.Height;
            this.dest = newDest;

            location = new Vector2(newDest.X, newDest.Y);
            objectSize = new Vector2(newDest.Width, newDest.Height);
        }

        public void setLocation(Vector2 location)
        {
            this.location.X = location.X;
            this.location.Y = location.Y;
            dest.X = (int)location.X;
            dest.Y = (int)location.Y;
        }

        public void setTexture(Texture2D texture)
        {
            this.texture = texture;
            bgTextureSet = true;
        }

        public void moveLocationBy(Vector2 translation)
        {
            setLocation(getLocation() + translation);
        }

        public void setSize(Vector2 dimensions)
        {
            this.objectSize.X = dimensions.X;
            this.objectSize.Y = dimensions.Y;
            dest.Width = (int)objectSize.X;
            dest.Height = (int)objectSize.Y;
        }

        public Vector2 getSize()
        {
            return objectSize;
        }

        public Vector2 getLocation()
        {
            return location;
        }

        public Rectangle getRect()
        {
            return dest;
        }

        public void setVisible(bool visible)
        {
            this.visible = visible;
        }

        public bool isVisible()
        {
            return visible;
        }

        public virtual void setFocus(bool hasFocus)
        {
            this.hasFocus = hasFocus;
        }

        public bool getHasFocus()
        {
            return hasFocus;
        }

        public virtual void next()
        {

        }

        public virtual void previous()
        {

        }

        public virtual void mouseMoved(Point oldP, Point newP)
        {

        }

        public virtual void mouseClicked(Point p)
        {

        }
    }
}
