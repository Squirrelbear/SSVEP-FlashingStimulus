using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI
{
    public class TextInput : Label
    {
        protected int cursorPos;
        protected KeySequenceListener keys; 
        protected bool _isChanged;
        protected Color baseColor, selectedColor;

        public TextInput(InputManager inputManager, Rectangle dest, string text, SpriteFont font)
            : this(inputManager, dest, text, font, Color.Black, false)
        {
        }

        public TextInput(InputManager inputManager, Rectangle dest, string text, SpriteFont font, bool multiline)
            : this(inputManager, dest, text, font, Color.Black, multiline)
        {
        }

        public TextInput(InputManager inputManager, Rectangle dest, string text, SpriteFont font, Color fontColor)
            : this(inputManager, dest, text, font, fontColor, false)
        {
        }

        public TextInput(InputManager inputManager, Rectangle dest, string text, SpriteFont font, Color fontColor, bool multiline)
            : base(dest, text, font, fontColor, multiline)
        {
            cursorPos = text.Length;
            keys = new KeySequenceListener(KeySequenceListener.ListenMode.All, inputManager);
            keys.start();
            _isChanged = false;
            baseColor = fontColor;
            selectedColor = Color.Red;
        }

        public override void update(GameTime gameTime)
        {
            _isChanged = false;

            if (getHasFocus())
            {
                keys.update(gameTime);
                if (keys.isBackspace() && getText().Length > 0)
                {
                    setText(getText().Substring(0, getText().Length - 1));
                    centreInRect();
                    _isChanged = true;
                }

                if (keys.peekString().Length > 0)
                {
                    string newS = getText() + keys.popString();
                    if (validateString(newS))
                    {
                        setText(newS);
                        centreInRect();
                        _isChanged = true;
                    }
                }
            }

            base.update(gameTime);
        }

        public override void mouseClicked(Point p)
        {
            base.mouseClicked(p);

            setFocus(dest.Contains(p));
        }

        public override void setFocus(bool hasFocus)
        {
            base.setFocus(hasFocus);

            if (hasFocus)
            {
                setColor(selectedColor);
            }
            else
            {
                setColor(baseColor);
            }
        }

        public bool getIsChanged()
        {
            return _isChanged;
        }

        public virtual bool validateString(string s)
        {
            int num;
            if (!int.TryParse(s, out num))
                return false;

            if (num <= 0 || num > 99)
                return false;

            return true;
        }
    }
}
