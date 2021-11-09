using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI
{
    public class CheckBox : WndComponent
    {
        Button check;
        Label text;
        bool stateChanged;

        public CheckBox(Rectangle dest, Texture2D selected, Texture2D unselected, string text, SpriteFont font)
            : base(dest)
        {
            check = new Button(new Rectangle(dest.X, dest.Y, dest.Height, dest.Height), selected, unselected);
            check.setSelected(true);
            this.text = new Label(new Rectangle(dest.X + dest.Height + 5, dest.Y, dest.Width - dest.Height, dest.Height), text, font);
            this.text.centreInRect(Label.CentreMode.CentreVertical);
            stateChanged = false;
        }

        public override void update(GameTime gameTime)
        {
            stateChanged = false;
            base.update(gameTime);

            check.update(gameTime);
            text.update(gameTime);
        }

        public override void draw(SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);

            if (visible)
            {
                check.draw(spriteBatch);
                text.draw(spriteBatch);
            }
        }

        public override void mouseClicked(Point p)
        {
            check.mouseClicked(p);
            if (check.getIsClicked())
            {
                next();
            }
        }

        public override void next()
        {
            check.setSelected(!check.getSelected());
            stateChanged = true;
        }

        public override void previous()
        {
            next();
        }

        public bool isSelected()
        {
            return check.getSelected();
        }

        public bool getChanged()
        {
            return stateChanged;
        }
    }
}
