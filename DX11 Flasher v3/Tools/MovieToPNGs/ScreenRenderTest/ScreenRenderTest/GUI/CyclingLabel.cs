using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System.Collections.Generic;

namespace GUI
{
    public class CyclingLabel : Label
    {
        protected int curID;
        protected bool random;
        protected Random gen;
        protected List<string> textList;
        protected Timer timer;
        protected bool autoForward;

        public CyclingLabel(Rectangle dest, List<string> textList, SpriteFont font, bool random)
            : this(dest, textList, font, Color.Black, random)
        {
        }

        public CyclingLabel(Rectangle dest, List<string> textList, SpriteFont font, Color fontColor, bool random)
            : base(dest, textList[0], font, fontColor)
        {
            curID = 0;
            this.random = random;
            this.textList = textList;
            if (random)
            {
                gen = new Random();
                setRandomText();
            }

            timer = null;
            autoForward = true;
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);

            if (timer != null)
            {
                timer.update(gameTime);
                if (timer.wasTriggered())
                {
                    if (autoForward)
                        nextText();
                    else
                        previousText();
                }
            }
        }

        public void setRandomText()
        {
            curID = gen.Next(textList.Count);
            setText(textList[curID]);
        }

        public void setSelection(int nextID)
        {
            this.curID = nextID;
            setText(textList[curID]);
        }

        public void nextText()
        {
            if (random)
            {
                setRandomText();
            }
            else
            {
                curID++;
                if (curID >= textList.Count)
                    curID = 0;
                setText(textList[curID]);
            }
        }

        public void previousText()
        {
            // note that this does not do it for random
            if (random)
            {
                setRandomText();
            }
            else
            {
                curID--;
                if (curID < 0)
                    curID = textList.Count - 1;
                setText(textList[curID]);
            }
        }

        public int getElementID()
        {
            return curID;
        }

        public string getElementString()
        {
            return textList[curID];
        }

        public int getTotalElements()
        {
            return textList.Count;
        }

        public void addText(string newText)
        {
            textList.Add(newText);
        }

        public void setTimer(Timer timer)
        {
            setTimer(timer, true);
        }

        public void setTimer(Timer timer, bool loopForward)
        {
            this.timer = timer;
            autoForward = loopForward;
        }
    }
}
