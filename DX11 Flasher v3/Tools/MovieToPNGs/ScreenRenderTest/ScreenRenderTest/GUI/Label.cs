using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System.Collections.Generic;

namespace GUI
{
    public class Label : WndComponent
    {
        public enum CentreMode { CentreVertical, CentreHorizontal, CentreBoth };

        protected string text;
        protected SpriteFont font;
        protected Color fontColor;
        protected Vector2 textPos;
        protected bool isMultiline;
        protected bool wordWrap;
        protected List<string> splitStrings;

        public Label(Rectangle dest, string text, SpriteFont font)
            : this(dest, text, font, Color.Black, false)
        {
        }

        public Label(Rectangle dest, string text, SpriteFont font, bool multiline)
            : this(dest, text, font, Color.Black, multiline)
        {
        }

        public Label(Rectangle dest, string text, SpriteFont font, Color fontColor)
            : this(dest, text, font, fontColor, false)
        {
        }

        public Label(Rectangle dest, string text, SpriteFont font, Color fontColor, bool multiline)
            : base(dest)
        {
            this.isMultiline = multiline;
            wordWrap = false;
            this.font = font;
            this.fontColor = fontColor;

            textPos = new Vector2(dest.X, dest.Y);
            setText(text);
        }

        public override void draw(SpriteBatch spriteBatch)
        {
            base.draw(spriteBatch);

            if (!isMultiline)
                spriteBatch.DrawString(font, text, textPos, fontColor);
            else
                drawMultiline(spriteBatch);
        }

        public void setText(string text)
        {
            this.text = text;
            if (isMultiline)
            {
                updateMultiline();
            }
        }

        public string getText()
        {
            return text;
        }

        public void setWordWrap(bool enabled)
        {
            this.wordWrap = enabled;
            updateMultiline();
        }

        public void setColor(Color color)
        {
            this.fontColor = color;
        }

        public override void setRect(Rectangle newRect)
        {
            base.setRect(newRect);

            textPos = new Vector2(newRect.X, newRect.Y);
        }

        public void centreInRect()
        {
            centreInRect(dest, CentreMode.CentreBoth);
        }

        public void centreInRect(CentreMode mode)
        {
            centreInRect(dest, mode);
        }

        public void centreInRect(Rectangle dest)
        {
            centreInRect(dest, CentreMode.CentreBoth);
        }

        public void centreInRect(Rectangle dest, CentreMode mode)
        {
            if (isMultiline)
            {
                int height = font.LineSpacing * splitStrings.Count;
                int maxChars = 0;
                string maxS = "";
                foreach (string s in splitStrings)
                {
                    if (s.Length > maxChars)
                    {
                        maxChars = s.Length;
                        maxS = s;
                    }
                }
                int width = (int)font.MeasureString(maxS).X;

                Point centre = dest.Center;
                int posX = (mode != CentreMode.CentreVertical) ? centre.X - width / 2 : dest.X;
                int posY = (mode != CentreMode.CentreHorizontal) ? centre.Y - height / 2 : dest.Y;
                textPos = new Vector2(posX, posY); 
            }
            else
            {
                Vector2 stringDims = font.MeasureString(text);
                Point centre = dest.Center;
                int posX = (mode != CentreMode.CentreVertical) ? centre.X - (int)stringDims.X / 2 : dest.X;
                int posY = (mode != CentreMode.CentreHorizontal) ? centre.Y - (int)stringDims.Y / 2 : dest.Y;
                textPos = new Vector2(posX, posY); 
            }
        }

        private void updateMultiline()
        {
            splitStrings = new List<string>();
            if (text.Length == 0)
            {
                return;
            }

            int lineHeight = font.LineSpacing;
            Vector2 curPos = new Vector2(textPos.X, textPos.Y);
            string remainingText = text;

            do{
                // if there is no text left or there is no more room in the block
                if(remainingText.Length == 0 || dest.Bottom  < curPos.Y + lineHeight) 
                {
                    break;
                }

                // determine number of characters that can fit
                // start by assuming that all the remaining text can fit
                string trialString = remainingText;
                do{
                    if(font.MeasureString(trialString).X > dest.Width)
                    {
                        if(wordWrap)
                        {
                            int spaceIndex = trialString.LastIndexOf(" ");
                            string word = trialString.Substring(spaceIndex);

                            // if the word to wrap is too big to wrap then use the default method
                            if(font.MeasureString(word).X > dest.Width)
                            {
                                trialString = trialString.Substring(0, trialString.Length-1);
                            }
                            else
                            {
                                trialString = trialString.Substring(0, trialString.Length - word.Length);
                            }
                        }
                        else
                        {
                            trialString = trialString.Substring(0, trialString.Length-1);
                        }
                    }
                    else
                    {
                        break;
                    }
                } while(true);

                // add the string and update variables
                splitStrings.Add(trialString);
                remainingText = remainingText.Substring(trialString.Length);
                curPos.Y += lineHeight;
            } while(true);
        }

        private void drawMultiline(SpriteBatch spriteBatch)
        {
            if (splitStrings.Count == 0) return;

            int lineHeight = font.LineSpacing;
            Vector2 curPos = new Vector2(textPos.X, textPos.Y);

            foreach (string s in splitStrings)
            {
                spriteBatch.DrawString(font, s, curPos, fontColor);
                curPos.Y += lineHeight;
            }
        }
    }
}
