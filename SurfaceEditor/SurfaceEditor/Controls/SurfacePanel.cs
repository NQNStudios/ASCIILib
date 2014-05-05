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
        public static Color INFO_COLOR = Color.FromArgb(128, 255, 255, 0);

        Graphics graphics;
        Font font;
        Surface surface;

        Point selectedCell;
        Point selectionSize = new Point(1, 1);

        ToolTip toolTip1;

        bool clicked = false;

        public InputMode Mode = InputMode.None;

        #region Initialization

        public SurfacePanel()
        {
            InitializeComponent();

            toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 0;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
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
            set
            { 
                surface = value;

                if (surface != null)
                {
                    //prepare the editor now that a new surface has been loaded.
                    (Parent as EditorForm).SpecialInfoControl.ClearLabels();
                    (Parent as EditorForm).SpecialInfoControl.LoadLabels(surface);
                }
            }
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
                
                if (surface.IsInBounds(value))
                {
                    //draw a highlight on the selected cell
                    DrawCursor(value);

                    //highlight its special info
                    if (Mode == InputMode.SpecialInfo)
                    {
                        string label = surface.GetSpecialInfo(value.X, value.Y);
                        
                        toolTip1.SetToolTip(this, label);
                    }
                    
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

                RefreshRect(new Rectangle(0, 0, surface.Width, surface.Height));
            }
        }

        #endregion

        #region Input Handling

        private void SurfacePanel_MouseMove(object sender, MouseEventArgs e)
        {
            CellInfo cellInfo1 = (Parent as EditorForm).CellInfo;

            Point cell = new Point(e.X / SurfacePanel.CHAR_WIDTH, e.Y / SurfacePanel.CHAR_HEIGHT);

            if (cellInfo1.Surface != null && surface.IsInBounds(cell))
            {
                cellInfo1.ShowCellInfo(cell.X, cell.Y);
            }
            else
            {
                cellInfo1.ClearCellInfo();
            }

            if (surface != null && cell != SelectedCell)
            {
                SelectedCell = cell;
            }
        }

        private void SurfacePanel_MouseLeave(object sender, EventArgs e)
        {
            SelectedCell = new Point(-1, -1);
        }

        private void SurfacePanel_MouseDown(object sender, MouseEventArgs e)
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

            FillRectangle(new Rectangle(x, y, CHAR_WIDTH * w, CHAR_HEIGHT * h), CURSOR_COLOR);
        }

        private void RefreshRect(Rectangle rect)
        {
            //draw all background colors
            for (int y = rect.Y; y < rect.Bottom && y < surface.Height; ++y)
            {
                int x = rect.X;

                while (x < rect.Right && x < surface.Width)
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
                            FillRectangle(new Rectangle(x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT), BACKGROUND_COLOR);
                            ++x;
                            break;
                        }

                        colorRect.Width += CHAR_WIDTH;
                        ++x;
                    } while (x < rect.Right && x < surface.Width && surface.GetBackgroundColor(x, y) == backgroundColor);

                    FillRectangle(colorRect, backgroundColor);
                }
            }

            //draw all characters
            for (int y = rect.Y; y < rect.Bottom && y < surface.Height; ++y)
            {
                int x = rect.X;

                while (x < rect.Right && x < surface.Width)
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
                    } while (x < rect.Right && x < surface.Width && surface.GetCharacterColor(x, y) == characterColor && surface.GetCharacter(x, y) != ' ' && surface.IsCellOpaque(x, y));

                    graphics.DrawString(text, font, new SolidBrush(characterColor), new PointF(destx - 2, desty));
                }
            }

            //draw special info overlays
            if (Mode == InputMode.SpecialInfo)
            {
                for (int c = rect.X; c < rect.Right && c < surface.Width; ++c)
                {
                    for (int r = rect.Y; r < rect.Bottom && r < surface.Height; ++r)
                    {
                        string label = surface.GetSpecialInfo(c, r);

                        if (label.Length > 0)
                        {
                            FillRectangle(new Rectangle(c * CHAR_WIDTH, r * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT), INFO_COLOR);
                        }
                    }
                }
            }
        }

        #endregion
    }
}
