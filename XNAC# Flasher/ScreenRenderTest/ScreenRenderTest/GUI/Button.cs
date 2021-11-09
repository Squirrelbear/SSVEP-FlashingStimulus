using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace GUI
{
    public class Button : WndComponent
    {
        protected Texture2D selected, unselected;
        protected bool isSelected;
        protected int actionID;
        protected bool isClicked;

        public Button(Rectangle dest, Texture2D selected, Texture2D unselected)
            : this(dest, selected, unselected, false, 0)
        {
        }

        public Button(Rectangle dest, Texture2D selected, Texture2D unselected, bool isSelected, int actionID)
            : base(dest, ((isSelected) ? selected : unselected))
        {
            this.isSelected = isSelected;
            this.selected = selected;
            this.unselected = unselected;
            this.actionID = actionID;
        }

        public override void draw(SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);

            isClicked = false;
        }

        public override void mouseClicked(Point p)
        {
            isClicked = dest.Contains(p);
        }

        #region Get/Set methods
        public void setSelected(bool isSelected)
        {
            this.isSelected = isSelected;

            texture = (isSelected) ? selected : unselected;
        }

        public bool getSelected()
        {
            return isSelected;
        }

        public void setActionID(int actionID)
        {
            this.actionID = actionID;
        }

        public int getActionID()
        {
            return actionID;
        }

        public bool getIsClicked()
        {
            return isClicked;
        }
        #endregion
    }
}
