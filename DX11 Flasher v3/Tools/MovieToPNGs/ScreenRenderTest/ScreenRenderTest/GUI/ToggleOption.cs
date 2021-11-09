using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI
{
    public class ToggleOption : WndComponent
    {
        protected bool loop;
        protected WndComponent arrowLeft, arrowRight;
        protected CyclingLabel text;
        protected bool isChanged;

        public ToggleOption(Rectangle dest, List<string> items, Texture2D arrowLeft, Texture2D arrowRight, SpriteFont font)
            : this(dest, items, arrowLeft, arrowRight, font, Color.Black)
        {
        }

        public ToggleOption(Rectangle dest, List<string> items, Texture2D arrowLeft, Texture2D arrowRight, SpriteFont font, Color fontColor)
            : base(dest)
        {            
            int arrowWidth = (int)calcArrowWidth(dest, arrowLeft);
            Rectangle arrowLeftDest = new Rectangle(dest.X, dest.Y, arrowWidth, dest.Height);
            this.arrowLeft = new WndComponent(arrowLeftDest, arrowLeft);
            Rectangle arrowRightDest = new Rectangle(dest.Right-arrowWidth, dest.Y, arrowWidth, dest.Height);
            this.arrowRight = new WndComponent(arrowRightDest, arrowRight);

            Rectangle textDest = new Rectangle(arrowLeftDest.Right + 10, arrowLeftDest.Y + 5, 
                                                dest.Width - arrowLeftDest.Width * 2 - 20, 
                                                dest.Height);
            text = new CyclingLabel(textDest, items, font, fontColor, false);
            text.centreInRect();
            text.setColor(Color.Maroon);
            isChanged = false;
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);

            isChanged = false;
        }

        public override void draw(SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);

            if(loop || text.getElementID() != 0)
                arrowLeft.draw(spriteBatch);
            if (loop || text.getElementID() != text.getTotalElements() - 1)
                arrowRight.draw(spriteBatch);
            text.draw(spriteBatch);
        }

        public void setSelection(int selectID)
        {
            text.setSelection(selectID);
            text.centreInRect(text.getRect());
        }

        public void setLoop(bool loop)
        {
            this.loop = loop;
        }

        public override void next()
        {
            if (text.getElementID() == text.getTotalElements() - 1 && !loop)
                return;

            text.nextText();
            text.centreInRect(text.getRect());

            isChanged = true;
        }

        public override void previous()
        {
            if (text.getElementID() == 0 && !loop)
                return;

            text.previousText();
            text.centreInRect(text.getRect());

            isChanged = true;
        }

        public int getSelectedID()
        {
            return text.getElementID();
        }

        public string getSelectedText()
        {
            return text.getElementString();
        }

        private float calcArrowWidth(Rectangle dest, Texture2D arrow)
        {
            float scale = arrow.Height * 1.0f / arrow.Width;
            return scale * dest.Height;
        }

        public override void mouseClicked(Point p)
        {
            base.mouseClicked(p);

            if (arrowLeft.getRect().Contains(p))
                previous();
            else if (arrowRight.getRect().Contains(p))
                next();
        }

        public bool getIsChanged()
        {
            return isChanged;
        }
    }
}
