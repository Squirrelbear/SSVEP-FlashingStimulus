using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace GUI
{
    public class InputManager
    {
        public enum InputDevice { Mouse, Gamepad, Keyboard };

        private KeyboardState oldKbState;
        private KeyboardState newKbState;
        private GamePadState[] oldGpState;
        private GamePadState[] newGpState;
        private MouseState oldMouseState;
        private MouseState mouseState;

        private int maxPlayers;
        private InputDevice[] playerInputMode;

        // hands
        private Point cursorPosition;
        private Point lastCursorPosition;
        private bool clicked;
        //private Vector2[] playerCursorPosition;

        public InputManager(int maxPlayers)
        {
            oldKbState = newKbState = Keyboard.GetState();

            this.maxPlayers = maxPlayers;

            if (maxPlayers < 1) maxPlayers = 1;
            if (maxPlayers > 4) maxPlayers = 4;

            oldGpState = new GamePadState[maxPlayers];
            newGpState = new GamePadState[maxPlayers];
            if(maxPlayers >= 1)
                oldGpState[0] = newGpState[0] = GamePad.GetState(PlayerIndex.One);
            if (maxPlayers >= 2)
                oldGpState[1] = newGpState[1] = GamePad.GetState(PlayerIndex.Two);
            if (maxPlayers >= 3)
                oldGpState[2] = newGpState[2] = GamePad.GetState(PlayerIndex.Three);
            if (maxPlayers >= 4)
                oldGpState[3] = newGpState[3] = GamePad.GetState(PlayerIndex.Four);

            playerInputMode = new InputDevice[maxPlayers];
            //playerCursorPosition = new Vector2[maxPlayers];
            for (int i = 0; i < maxPlayers; i++)
            {
                playerInputMode[i] = InputDevice.Keyboard;
                //playerCursorPosition[i] = new Vector2(0,0);
            }

            mouseState = Mouse.GetState();
            cursorPosition = new Point(mouseState.X, mouseState.Y);
            lastCursorPosition = new Point(mouseState.X, mouseState.Y);
            clicked = false;
        }

        public void update(GameTime gameTime)
        {
            oldKbState = newKbState;
            newKbState = Keyboard.GetState();

            for(int i = 0; i < maxPlayers; i++)
                oldGpState[i] = newGpState[i];

            if (maxPlayers >= 1)
                newGpState[0] = GamePad.GetState(PlayerIndex.One);
            if (maxPlayers >= 2)
                newGpState[1] = GamePad.GetState(PlayerIndex.Two);
            if (maxPlayers >= 3)
                newGpState[2] = GamePad.GetState(PlayerIndex.Three);
            if (maxPlayers >= 4)
                newGpState[3] = GamePad.GetState(PlayerIndex.Four);

            oldMouseState = mouseState;
            mouseState = Mouse.GetState();
            lastCursorPosition = cursorPosition;
            cursorPosition = new Point(mouseState.X, mouseState.Y);
            clicked = (mouseState.LeftButton == ButtonState.Pressed && oldMouseState.LeftButton == ButtonState.Released);

            //cursorPosition.X += newGpState.ThumbSticks.Left.X * 200 * gameTime.ElapsedGameTime.Milliseconds / 1000.0f;
            //cursorPosition.Y -= newGpState.ThumbSticks.Left.Y * 200 * gameTime.ElapsedGameTime.Milliseconds / 1000.0f;
        }

        #region Mouse Functions
        public bool hasMouseMoved()
        {
            return lastCursorPosition.X != cursorPosition.X || lastCursorPosition.Y != cursorPosition.Y;
        }

        public Point getCursor()
        {
            return cursorPosition;
        }

        public Point getLastCursor()
        {
            return lastCursorPosition;
        }

        public bool getMouseClicked()
        {
            return clicked;//mouseState.LeftButton == ButtonState.Pressed && oldMouseState.LeftButton == ButtonState.Released;
        }
        #endregion

        #region KeyboardFunctions
        public bool isKeyPressed(Keys key)
        {
            if (oldKbState.IsKeyDown(key) && newKbState.IsKeyUp(key)) return true;
            return false;
        }

        public bool isKeyReleased(Keys key)
        {
            if (newKbState.IsKeyDown(key) && oldKbState.IsKeyUp(key)) return true;
            return false;
        }

        public bool isKeyDown(Keys key)
        {
            return newKbState.IsKeyDown(key);
        }
        #endregion

        #region GamePadFunctions
        public GamePadState getGamePadState(int player)
        {
            return newGpState[player - 1];
        }

        public GamePadState getOldGamePadState(int player)
        {
            return oldGpState[player - 1];
        }
        
        public bool isBtnPressed(Buttons btn, int player)
        {
            if (getOldGamePadState(player).IsButtonDown(btn) && getGamePadState(player).IsButtonUp(btn)) return true;
            return false;
        }

        public bool isBtnReleased(Buttons btn, int player)
        {
            if (getGamePadState(player).IsButtonDown(btn) && getOldGamePadState(player).IsButtonUp(btn)) return true;
            return false;
        }

        public bool isBtnDown(Buttons btn, int player)
        {
            return getGamePadState(player).IsButtonDown(btn);
        }

        public float getTriggerState(bool isRight, int player)
        {
            if (isRight) return getGamePadState(player).Triggers.Right;
            else return getGamePadState(player).Triggers.Left;
        }

        public float getThumbStickStateRightX(int player)
        {
            return getGamePadState(player).ThumbSticks.Right.X;
        }

        public float getThumbStickStateRightY(int player)
        {
            return getGamePadState(player).ThumbSticks.Right.Y;
        }

        public float getThumbStickStateLeftX(int player)
        {
            return getGamePadState(player).ThumbSticks.Left.X;
        }

        public float getThumbStickStateLeftY(int player)
        {
            return getGamePadState(player).ThumbSticks.Left.Y;
        }

        public bool isGamePadConnected(int player)
        {
            return getGamePadState(player).IsConnected;
        }
        #endregion
        
        #region HelperFunctions
        public float rescale(float value, float rangeMin, float rangeMax, float targetMin, float targetMax)
        {
            float scale = (rangeMax - rangeMax) / (targetMax - targetMin);
            return targetMin + (value - rangeMin) / scale;
        }

        public float rescaleWidth(float value, float targetMin, float targetMax)
        {
            return rescale(value, 0, 1024, targetMin, targetMax);
        }

        public float rescaleHeight(float value, float targetMin, float targetMax)
        {
            return rescale(value, 0, 768, targetMin, targetMax);
        }
        #endregion
    }
}
