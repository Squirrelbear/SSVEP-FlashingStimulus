using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GUI;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.IO;

namespace ScreenRenderTest
{
    public class OptionDialog : WndHandle
    {
        Panel optionPanel;
        CheckBox showDialogCheck;
        ToggleOption backgroundViewOption;
        ToggleOption flashColourOption;
        ToggleOption flashRateOption;
        ToggleOption flashModeOption;
        ToggleOption vSynchOption;
        ToggleOption fpsOption;
        TextButton save, load;
        TextInput heightInput, widthInput;

        public OptionDialog(Rectangle displayRect, ScreenRenderTest.Game1 appRef)
            : base(WndType.Options, displayRect, appRef)
        {
            Texture2D checkBox = loadTexture("WndContent\\Components\\Unchecked");
            Texture2D checkBoxSelected = loadTexture("WndContent\\Components\\checked");
            Texture2D arrowLeft = loadTexture("WndContent\\Components\\Left");
            Texture2D arrowRight = loadTexture("WndContent\\Components\\Right");

            SpriteFont font = loadFont("smallFont");
            int height = (int)font.MeasureString("Show/Hide Options").Y + 10;
            int width = (int)font.MeasureString("Show/Hide Options").X + height + 50;
            int maxOptions = 15;//displayRect.Height / height; 
            Rectangle actualRect = new Rectangle(displayRect.X + 3, displayRect.Y, /*displayRect.Width*/width, height * maxOptions);
            Rectangle bgRect = new Rectangle(displayRect.X - 2, displayRect.Y - 2, /*displayRect.Width*/width + 10, actualRect.Height + 10 + 5);
            displayRect = bgRect;

            LayoutManger lM = new LayoutManger(actualRect, maxOptions, 1);

            showDialogCheck = new CheckBox(lM.nextRect(), checkBoxSelected, checkBox, "Show/Hide Options", font);

            Texture2D bgTexture = loadTexture("dialogbg");
            optionPanel = new Panel(new Rectangle(displayRect.X, displayRect.Y,
                                        displayRect.Width, displayRect.Height), bgTexture);

            Label backgroundViewLabel = new Label(lM.nextRect(), "Background:", font);
            backgroundViewLabel.centreInRect(Label.CentreMode.CentreVertical);
            List<string> backgrounds = new List<string>();
            backgrounds.Add("Plain White");
            backgrounds.Add("Plain Black");
            backgrounds.Add("Shade Change");
            backgrounds.Add("Gameplay Video");
            backgroundViewOption = new ToggleOption(lM.nextRect(), backgrounds, arrowLeft, arrowRight, font);
            backgroundViewOption.setSelection(3);

            Label flashColourLabel = new Label(lM.nextRect(), "Flash Colour:", font);
            flashColourLabel.centreInRect(Label.CentreMode.CentreVertical);
            List<string> flashColours = new List<string>();
            flashColours.Add("Black");
            flashColours.Add("White");
            flashColours.Add("Red");
            flashColours.Add("Green");
            flashColours.Add("Blue");
            flashColourOption = new ToggleOption(lM.nextRect(), flashColours, arrowLeft, arrowRight, font);

            Label flashRateLabel = new Label(lM.nextRect(), "Flash Rate (Hz):", font);
            flashRateLabel.centreInRect(Label.CentreMode.CentreVertical);
            List<string> flashIntervals = new List<string>();
            flashIntervals.Add("1");
            flashIntervals.Add("2");
            flashIntervals.Add("3");
            flashIntervals.Add("4");
            for (int i = 5; i <= 100; i += 5)
                flashIntervals.Add(i + "");
            flashRateOption = new ToggleOption(lM.nextRect(), flashIntervals, arrowLeft, arrowRight, font);

            Label flashModeLabel = new Label(lM.nextRect(), "Flash Mode:", font);
            flashModeLabel.centreInRect(Label.CentreMode.CentreVertical);
            List<string> flashModes = new List<string>();
            flashModes.Add("Graphics Clear");
            flashModes.Add("Rendered Texture");
            flashModes.Add("Texture Grid");
            flashModeOption = new ToggleOption(lM.nextRect(), flashModes, arrowLeft, arrowRight, font);

            LayoutManger gridSplit = new LayoutManger(lM.nextRect(), 1, 4);
            Label gridWidthLabel = new Label(gridSplit.nextRect(), "Grid W:", font);
            gridWidthLabel.centreInRect(Label.CentreMode.CentreVertical);
            widthInput = new TextInput(inputManager, gridSplit.nextRect(), "8", font);
            widthInput.centreInRect();
            Label gridHeightLabel = new Label(gridSplit.nextRect(), "Grid H:", font);
            gridHeightLabel.centreInRect(Label.CentreMode.CentreVertical);
            heightInput = new TextInput(inputManager, gridSplit.nextRect(), "8", font);
            heightInput.centreInRect();

            Label vSynchLabel = new Label(lM.nextRect(), "Vertical Retrace Synch:", font);
            vSynchLabel.centreInRect(Label.CentreMode.CentreVertical);
            List<string> vSynchOptions = new List<string>();
            vSynchOptions.Add("Disabled");
            vSynchOptions.Add("Enabled");
            vSynchOption = new ToggleOption(lM.nextRect(), vSynchOptions, arrowLeft, arrowRight, font);
            vSynchOption.setSelection(1);

            Label fpsLabel = new Label(lM.nextRect(), "Desired FPS:", font);
            fpsLabel.centreInRect(Label.CentreMode.CentreVertical);
            List<string> fpsRates = new List<string>();
            for (int i = 50; i <= 300; i += 50)
                fpsRates.Add(i + "");
            fpsOption = new ToggleOption(lM.nextRect(), fpsRates, arrowLeft, arrowRight, font);
            fpsOption.setSelection(fpsRates.Count - 2);

            Rectangle btnsRect = lM.nextRect();
            Rectangle saveRect = new Rectangle(btnsRect.X, btnsRect.Y + 5, btnsRect.Width / 2, btnsRect.Height);
            Rectangle loadRect = new Rectangle(btnsRect.X + btnsRect.Width / 2, btnsRect.Y + 5, btnsRect.Width / 2, btnsRect.Height);
            save = new TextButton(saveRect, "Save", font, bgTexture, bgTexture);
            load = new TextButton(loadRect, "Load", font, bgTexture, bgTexture);

            optionPanel.addComponent(backgroundViewLabel);
            optionPanel.addComponent(backgroundViewOption);
            optionPanel.addComponent(flashColourLabel);
            optionPanel.addComponent(flashColourOption);
            optionPanel.addComponent(flashRateLabel);
            optionPanel.addComponent(flashRateOption);
            optionPanel.addComponent(flashModeLabel);
            optionPanel.addComponent(flashModeOption);
            optionPanel.addComponent(gridWidthLabel);
            optionPanel.addComponent(widthInput);
            optionPanel.addComponent(gridHeightLabel);
            optionPanel.addComponent(heightInput);
            optionPanel.addComponent(vSynchLabel);
            optionPanel.addComponent(vSynchOption);
            optionPanel.addComponent(fpsLabel);
            optionPanel.addComponent(fpsOption);
            optionPanel.addComponent(save);
            optionPanel.addComponent(load);

            addComponent(optionPanel);
            addComponent(showDialogCheck);
        }

        public override void update(GameTime gameTime)
        {
            base.update(gameTime);

            if (showDialogCheck.getChanged())
                optionPanel.setVisible(showDialogCheck.isSelected());

            if (backgroundViewOption.getIsChanged())
                appRef.setBGMode((Game1.BackgroundMode)backgroundViewOption.getSelectedID());

            if (flashColourOption.getIsChanged())
            {
                switch (flashColourOption.getSelectedID())
                {
                    case 0:
                        appRef.setFlashColour(Color.Black);
                        break;
                    case 1:
                        appRef.setFlashColour(Color.White);
                        break;
                    case 2:
                        appRef.setFlashColour(Color.Red);
                        break;
                    case 3:
                        appRef.setFlashColour(Color.Green);
                        break;
                    case 4:
                        appRef.setFlashColour(Color.Blue);
                        break;
                }
            }

            if (flashRateOption.getIsChanged())
                appRef.setFlashHz(int.Parse(flashRateOption.getSelectedText()));

            if (flashModeOption.getIsChanged())
                appRef.setFlashMode((Flasher.FlashMode)flashModeOption.getSelectedID());

            if (vSynchOption.getIsChanged())
                appRef.setVSync((vSynchOption.getSelectedID() == 0) ? false : true);

            if (fpsOption.getIsChanged())
            {
                appRef.SetFrameRate(int.Parse(fpsOption.getSelectedText()));
                appRef.setVSync((vSynchOption.getSelectedID() == 0) ? false : true);
            }

            if (widthInput.getIsChanged() && widthInput.getText().Length > 0)
                appRef.setGridWidth(int.Parse(widthInput.getText()));
            if (heightInput.getIsChanged() && heightInput.getText().Length > 0)
                appRef.setGridHeight(int.Parse(heightInput.getText()));

            if (save.getIsClicked())
                saveOptions();

            if (load.getIsClicked())
                loadOptions();
        }

        public void saveOptions()
        {
            string fName = "settings.dat";
            TextWriter tw = new StreamWriter(fName);
            tw.Write(backgroundViewOption.getSelectedID() + ",");
            tw.Write(flashColourOption.getSelectedID() + ",");
            tw.Write(flashRateOption.getSelectedID() + ",");
            tw.Write(flashModeOption.getSelectedID() + ",");
            tw.Write(vSynchOption.getSelectedID() + ",");
            tw.Write(fpsOption.getSelectedID());
            tw.Close();

            optionPanel.addComponent(backgroundViewOption);
            optionPanel.addComponent(flashColourOption);
            optionPanel.addComponent(flashRateOption);
            optionPanel.addComponent(flashModeOption);
            optionPanel.addComponent(vSynchOption);
            optionPanel.addComponent(fpsOption);
        }

        public void loadOptions()
        {
            string fName = "settings.dat";
            try
            {
                TextReader tr = new StreamReader(fName);
                string line = tr.ReadToEnd();
                tr.Close();
                string[] data = line.Split(',');

                backgroundViewOption.setSelection(int.Parse(data[0]));
                flashColourOption.setSelection(int.Parse(data[1]));
                flashRateOption.setSelection(int.Parse(data[2]));
                flashModeOption.setSelection(int.Parse(data[3]));
                vSynchOption.setSelection(int.Parse(data[4]));
                fpsOption.setSelection(int.Parse(data[5]));
                widthInput.setText(data[6]);
                heightInput.setText(data[7]);

                appRef.setBGMode((Game1.BackgroundMode)backgroundViewOption.getSelectedID());
                appRef.setFlashHz(int.Parse(flashRateOption.getSelectedText()));
                appRef.setGridWidth(int.Parse(widthInput.getText()));
                appRef.setGridHeight(int.Parse(heightInput.getText()));
                appRef.setFlashMode((Flasher.FlashMode)flashModeOption.getSelectedID());
                appRef.SetFrameRate(int.Parse(fpsOption.getSelectedText()));
                appRef.setVSync((vSynchOption.getSelectedID() == 0) ? false : true);

                switch (flashColourOption.getSelectedID())
                {
                    case 0:
                        appRef.setFlashColour(Color.Black);
                        break;
                    case 1:
                        appRef.setFlashColour(Color.White);
                        break;
                    case 2:
                        appRef.setFlashColour(Color.Red);
                        break;
                    case 3:
                        appRef.setFlashColour(Color.Green);
                        break;
                    case 4:
                        appRef.setFlashColour(Color.Blue);
                        break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception loading file: " + e.Message);
            }
        }
    }
}
