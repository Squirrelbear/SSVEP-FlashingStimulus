using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI
{
    public class TabbedPanel : Panel
    {
        protected List<Panel> panels;
        protected int curPanel;

        public TabbedPanel(Rectangle dest)
            : base(dest)
        {
            panels = new List<Panel>();
        }

        public TabbedPanel(Rectangle dest, Texture2D background)
            : base(dest, background)
        {
            panels = new List<Panel>();
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);

            if(curPanel < panels.Count)
                panels[curPanel].update(gameTime);
        }

        public override void draw(Microsoft.Xna.Framework.Graphics.SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);
            
            if(curPanel < panels.Count)
                panels[curPanel].draw(spriteBatch);
        }

        public void addPanel(Panel panel)
        {
            panels.Add(panel);
        }

        public void removePanel(int id)
        {
            panels.RemoveAt(id);
        }

        public List<Panel> getPanels()
        {
            return panels;
        }

        public void setPanel(int panelID)
        {
            this.curPanel = panelID;
        }
    }
}
