using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace GUI
{
    public class TextButton : Button
    {
        protected string text;
        protected SpriteFont font;
        protected Color fontColor;
        protected Color fontColorSelected;
        protected Vector2 textPosition;

        public TextButton(Rectangle dest, string text, SpriteFont font, Texture2D selected, Texture2D unselected)
            : this(dest, text, font, selected, unselected, false, 0)
        {
        }

        public TextButton(Rectangle dest, string text, SpriteFont font, Texture2D selected, Texture2D unselected, bool isSelected, int actionID)
            : base(dest, selected, unselected, isSelected, actionID)
        {
            this.text = text;
            this.font = font;
            fontColor = Color.MidnightBlue;
            fontColorSelected = Color.MidnightBlue;

            Vector2 stringDims = font.MeasureString(text);
            Point centre = dest.Center;
            textPosition = new Vector2(centre.X - stringDims.X / 2, centre.Y - stringDims.Y / 2); 
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);
        }

        public override void draw(SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);

            if (!visible) return;

            if(isSelected)
                spriteBatch.DrawString(font, text, textPosition, fontColorSelected);
            else
                spriteBatch.DrawString(font, text, textPosition, fontColor);
        }

        public void setFontColor(Color color)
        {
            this.fontColor = color;
        }

        public void setSelectedFontColor(Color color)
        {
            this.fontColorSelected = color;
        }
    }
}
