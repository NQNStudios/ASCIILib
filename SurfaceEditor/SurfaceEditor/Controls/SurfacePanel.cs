using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;

namespace SurfaceEditor
{
    /// <summary>
    /// Panel which displays the surface being edited.
    /// </summary>
    public partial class SurfacePanel : UserControl
    {
        public enum InputMode
        {
            ToggleOpacity,
            PaintBrush,
            SmallText,
            LongText,
            SpecialInfo,
            None
        }

        public const int CHAR_WIDTH = 8;
        public const int CHAR_HEIGHT = 12;
        public static Color BACKGROUND_COLOR = Color.Gray;
        public static Color CURSOR_COLOR = Color.FromArgb(128, 255, 0, 0);

        Graphics graphics;
        Font font;
        Surface surface;

        Point selectedCell;
        Point selectionSize = new Point(1, 1);

        bool clicked = false;

        public InputMode Mode = InputMode.None;

        #region Initialization

        public SurfacePanel()
        {
            InitializeComponent();
        }

        private void SurfacePanel_Load(object sender, EventArgs e)
        {
            graphics = this.CreateGraphics();

            font = new Font("The One True Font (System 8x12)", 12);
        }

        #endregion

        #region Properties

        public Surface Surface
        {
            get { return surface; }
            set { surface = value; }
        }

        public Point SelectedCell
        {
            get { return selectedCell; }
            set
            {
                if (surface == null) return;

                //if a place was previously selected, draw over its highlight
                if (surface.IsInBounds(selectedCell))
                {
                    int x = selectedCell.X;
                    int y = selectedCell.Y;

                    RefreshRect(new Rectangle(x, y, selectionSize.X, selectionSize.Y));
                }
                
                //draw a highlight on the selected cell
                if (surface.IsInBounds(value))
                {
                     DrawCursor(value);
                }

                selectedCell = value;
            }
        }

        public Point SelectionSize
        {
            get { return selectionSize; }
            set { selectionSize = value; }
        }

        public bool Clicked
        {
            get { return clicked; }
            set { clicked = value; }
        }

        #endregion

        #region Rendering

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Clear();

            if (surface != null)
            {
                //Draw the loaded surface

                //draw all background colors
	            for (int y = 0; y < surface.Height; ++y)
	            {
		            int x = 0;

		            while (x < surface.Width)
		            {
			            //chain all adjacent background colors in a row for more efficient rendering
			            Rectangle colorRect = new Rectangle();

			            colorRect.X = x * CHAR_WIDTH;
			            colorRect.Y = y * CHAR_HEIGHT;
			            colorRect.Width = 0;
			            colorRect.Height = CHAR_HEIGHT;

			            Color backgroundColor = surface.GetBackgroundColor(x, y);

			            do
			            {
                            if (!surface.IsCellOpaque(x, y))
                            {
                                ++x;
                                break;
                            }

				            colorRect.Width += CHAR_WIDTH;
				            ++x;
			            } while (x < surface.Width && surface.GetBackgroundColor(x, y) == backgroundColor);

                        graphics.FillRectangle(new SolidBrush(backgroundColor), colorRect);
		            }
	            }

	            //draw all characters
	            for (int y = 0; y < surface.Height; ++y)
	            {
		            int x = 0;

		            while (x < surface.Width)
		            {
			            //chain all adjacent characters with the same color into strings for more efficient rendering

			            char ch = surface.GetCharacter(x, y);
			            if (ch == ' ')
			            {
				            ++x;
				            continue;
			            }

			            int destx = x * CHAR_WIDTH;
			            int desty = y * CHAR_HEIGHT;
			            
                        string text = "";
			            Color characterColor = surface.GetCharacterColor(x, y);

			            do
			            {
                            if (!surface.IsCellOpaque(x, y))
                            {
                                ++x;
                                break;
                            }

				            char character = surface.GetCharacter(x, y);
				            text += character;
				            ++x;
                        } while (x < surface.Width && surface.GetCharacterColor(x, y) == characterColor && surface.GetCharacter(x, y) != ' ' && surface.IsCellOpaque(x, y));

			            graphics.DrawString(text, font, new SolidBrush(characterColor), new PointF(destx - 2, desty));
		            }
	            }
            }
        }

        #endregion

        #region Input Handling

        public void SurfacePanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (e == null || e.Button == MouseButtons.Left)
            {
                if (surface != null && surface.IsInBounds(SelectedCell))
                {
                    //handle input for all valid selected cells
                    for (int c = SelectedCell.X, i = 0; c < surface.Width && i < SelectionSize.X; ++c, ++i)
                    {
                        for (int r = SelectedCell.Y, j = 0; r < surface.Height && j < SelectionSize.Y; ++r, ++j)
                        {
                            //a cell was clicked on. Handle the appropriate behavior for the selected mode.
                            switch (Mode)
                            {
                                case InputMode.ToggleOpacity:
                                    surface.SetCellOpacity(c, r, !surface.IsCellOpaque(c, r));
                                    break;
                                case InputMode.PaintBrush:
                                    EditorForm parent = (Parent as EditorForm);
                                    BrushControl brushControl = parent.BrushControl;

                                    if (brushControl.PaintCharacter)
                                    {
                                        surface.SetCharacter(c, r, brushControl.Character);
                                    }
                                    if (brushControl.PaintBackgroundColor)
                                    {
                                        surface.SetBackgroundColor(c, r, brushControl.BackgroundColor);
                                    }
                                    if (brushControl.PaintCharacterColor)
                                    {
                                        surface.SetCharacterColor(c, r, brushControl.CharacterColor);
                                    }
                                    break;
                                case InputMode.SmallText:
                                    ShortTextForm form = new ShortTextForm();
                                    DialogResult result = form.ShowDialog(this);

                                    if (result == DialogResult.OK)
                                    {
                                        surface.BlitString(form.ChosenText, form.ChosenColor, c, r);
                                        Refresh();
                                    }
                                    break;
                                case InputMode.LongText:
                                    LongTextForm longTextForm = new LongTextForm();
                                    DialogResult longResult = longTextForm.ShowDialog(this);

                                    if (longResult == DialogResult.OK)
                                    {
                                        surface.BlitStringMultiline(longTextForm.ChosenText, longTextForm.ChosenColor, 
                                            c, r, selectionSize.X, selectionSize.Y);
                                        Refresh();
                                    }
                                    break;
                                case InputMode.SpecialInfo:
                                    string label = (Parent as EditorForm).SpecialInfoControl.SelectedLabel;
                                    if (label == "[Clear Info]")
                                    {
                                        label = "";
                                    }
                                    surface.SetSpecialInfo(c, r, label);
                                    break;
                            }
                            if (Mode == InputMode.LongText)
                            {
                                break; //stop recursing through the whole selection; it only has to happen once
                            }
                        }
                        if (Mode == InputMode.LongText)
                        {
                            break; //stop recursing through the whole selection; it only has to happen once
                        }
                    }
                    RefreshRect(new Rectangle(selectedCell.X, selectedCell.Y, selectionSize.X, selectionSize.Y));
                    DrawCursor(new Point(selectedCell.X, selectedCell.Y));
                }
            }
        }

        #endregion

        #region Graphics Helper Methods

        private void Clear()
        {
            graphics.Clear(BACKGROUND_COLOR);
        }

        private void FillRectangle(Rectangle rectangle, Color color)
        {
            Brush brush = new SolidBrush(color);

            graphics.FillRectangle(brush, rectangle);

            brush.Dispose();
        }

        private void DrawCursor(Point position)
        {
            int x = position.X * CHAR_WIDTH;
            int y = position.Y * CHAR_HEIGHT;

            int w = Math.Min(selectionSize.X, surface.Width - position.X);
            int h = Math.Min(selectionSize.Y, surface.Height - position.Y);

            graphics.FillRectangle(new SolidBrush(CURSOR_COLOR), new Rectangle(x, y, CHAR_WIDTH * w, CHAR_HEIGHT * h));
        }

        private void RefreshRect(Rectangle rect)
        {
            for (int i = 0; i < rect.Width; ++i)
            {
                for (int j = 0; j < rect.Height; ++j)
                {
                    int x = rect.X + i;
                    int y = rect.Y + j;

                    //draw over the old one
                    if (surface.IsInBounds(new Point(x, y)) && surface.IsCellOpaque(x, y))
                    {
                        Rectangle rectangle = new Rectangle(x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);

                        Brush brush = new SolidBrush(surface.GetBackgroundColor(x, y));

                        graphics.FillRectangle(brush, rectangle);
                        graphics.DrawString("" + surface.GetCharacter(x, y), font, new SolidBrush(surface.GetCharacterColor(x, y)), new PointF(rectangle.X - 2, rectangle.Y));
                    }
                    else
                    {
                        Rectangle rectangle = new Rectangle(x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);

                        graphics.FillRectangle(new SolidBrush(BACKGROUND_COLOR), rectangle);
                    }
                }
            }
        }

        #endregion
    }
}
