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
            Inspect,
            ToggleOpacity,
            PaintBrush,
            SmallText,
            LongText,
            SpecialInfo,
            FillCells,
            None,
            EyeDropper,
            Shapes
        }

        public const int CHAR_WIDTH = 8;
        public const int CHAR_HEIGHT = 12;
        public const float CHAR_OFFSET = -2.0f;

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

        public InputMode Mode = InputMode.Inspect;
        
        Point firstCorner = new Point(-1, -1); // for shapes

        Stack<List<CellChange>> actions = new Stack<List<CellChange>>();
        Stack<List<CellChange>> undoneActions = new Stack<List<CellChange>>();

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

            SelectedCell = new Point(-1, -1);
        }

        #endregion

        #region Properties

        public Surface Surface
        {
            get { return surface; }
            set
            { 
                surface = value;
            }
        }

        public Point SelectedCell
        {
            get { return selectedCell; }
            set
            {
                if (surface == null) return;

                value.X += hScrollBar1.Value / CHAR_WIDTH;
                value.Y += vScrollBar1.Value / CHAR_HEIGHT;

                //update the cursor text
                if (!surface.IsInBounds(value))
                {
                    (Parent as EditorForm).CursorLabel.Text = "";
                }
                else
                {
                    (Parent as EditorForm).CursorLabel.Text = "Selected Cell: " + value.X + ", " + value.Y + "    Selection Size: " + selectionSize.X + ", " + selectionSize.Y;
                }

                //if a place was previously selected, draw over its highlight
                if (surface.IsInBounds(selectedCell))
                {
                    int x = selectedCell.X;
                    int y = selectedCell.Y;

                    RefreshRect(new Rectangle(x, y, selectionSize.X, selectionSize.Y));
                }
                
                if (surface.IsInBounds(value) && Mode != InputMode.None)
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
                int dx = value.X - selectedCell.X;
                int dy = value.Y - selectedCell.Y;

                selectedCell = value;

                HandleDrag(dx, dy);
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

        private void HandleDrag(int dx, int dy)
        {
            //handle dragging click inputs
            if (Control.MouseButtons.HasFlag(MouseButtons.Left))
            {
                if (surface != null && surface.IsInBounds(SelectedCell))
                {
                    RefreshRect(new Rectangle(selectedCell.X - dx, selectedCell.Y - dy, selectionSize.X, selectionSize.Y));

                    List<CellChange> changes = new List<CellChange>();

                    //handle input for all valid selected cells
                    for (int c = SelectedCell.X, i = 0; c < surface.Width && i < SelectionSize.X; ++c, ++i)
                    {
                        for (int r = SelectedCell.Y, j = 0; r < surface.Height && j < SelectionSize.Y; ++r, ++j)
                        {
                            if (c < 0) continue;
                            if (r < 0) continue;

                            bool valid = false;
                            if (dx > 0 && i < selectionSize.X - dx) valid = true;
                            if (dy > 0 && j < selectionSize.Y - dy) valid = true;
                            if (dx < 0 && selectionSize.X - i < -dx) valid = true;
                            if (dy < 0 && selectionSize.Y - j < -dy) valid = true;

                            if (dx == 0 || dy == 0) valid = true;

                            if (selectionSize == new Point(1, 1)) valid = true;

                            if (!valid) continue;

                            //a cell was clicked on. Handle the appropriate behavior for the selected mode.

                            bool changed = false;
                            CellChange change = new CellChange();
                            change.X = c;
                            change.Y = r;
                            change.OldCharacter = surface.GetCharacter(c, r);
                            change.OldBackgroundColor = surface.GetBackgroundColor(c, r);
                            change.OldCharacterColor = surface.GetCharacterColor(c, r);
                            change.OldOpacity = surface.IsCellOpaque(c, r);
                            change.OldSpecialInfo = surface.GetSpecialInfo(c, r);

                            change.NewCharacter = change.OldCharacter;
                            change.NewBackgroundColor = change.OldBackgroundColor;
                            change.NewCharacterColor = change.OldCharacterColor;
                            change.NewOpacity = change.OldOpacity;
                            change.NewSpecialInfo = change.OldSpecialInfo;

                            switch (Mode)
                            {
                                case InputMode.PaintBrush:
                                    EditorForm parent = (Parent as EditorForm);
                                    BrushControl brushControl = parent.BrushControl;

                                    if (brushControl.PaintCharacter)
                                    {
                                        change.NewCharacter = brushControl.Character;
                                        surface.SetCharacter(c, r, brushControl.Character);
                                        changed = true;
                                    }
                                    if (brushControl.PaintBackgroundColor)
                                    {
                                        change.NewBackgroundColor = brushControl.BackgroundColor;
                                        surface.SetBackgroundColor(c, r, brushControl.BackgroundColor);
                                        changed = true;
                                    }
                                    if (brushControl.PaintCharacterColor)
                                    {
                                        change.NewCharacterColor = brushControl.CharacterColor;
                                        surface.SetCharacterColor(c, r, brushControl.CharacterColor);
                                        changed = true;
                                    }
                                    break;
                                case InputMode.SpecialInfo:
                                    string label = (Parent as EditorForm).SpecialInfoControl.SelectedLabel;
                                    if (label == "[Clear Info]")
                                    {
                                        label = "";
                                    }
                                    change.NewSpecialInfo = label;
                                    surface.SetSpecialInfo(c, r, label);
                                    changed = true;
                                    break;
                            }

                            if (changed) changes.Add(change);
                        }
                    }
                    DrawCursor(new Point(selectedCell.X, selectedCell.Y));

                    AddAction(changes);
                }
            }
        }

        private void SurfacePanel_MouseLeave(object sender, EventArgs e)
        {
            if (!(Parent.Visible && !Parent.CanFocus))
            {
                // no modal child windows are open
                SelectedCell = new Point(-1, -1);
            }
        }

        private void SurfacePanel_MouseDown(object sender, MouseEventArgs e)
        {
            HandleDrag(0, 0);

            //handle one-time click inputs
            if (e.Button == MouseButtons.Left)
            {
                if (surface != null && surface.IsInBounds(SelectedCell))
                {
                    //handle input for all valid selected cells
                    int c = SelectedCell.X;
                    int r = SelectedCell.Y;

                    List<CellChange> changes = new List<CellChange>();

                    //a cell was clicked on. Handle the appropriate behavior for the selected mode.
                    switch (Mode)
                    {
                        case InputMode.ToggleOpacity:
                            for (int i = 0; i < selectionSize.X; ++i)
                            {
                                for (int j = 0; j < selectionSize.Y; ++j)
                                {
                                    int x = c + i;
                                    int y = r + j;
                                    CellChange change = new CellChange();
                                    change.X = x;
                                    change.Y = y;
                                    change.OldCharacter = surface.GetCharacter(c, r);
                                    change.OldBackgroundColor = surface.GetBackgroundColor(c, r);
                                    change.OldCharacterColor = surface.GetCharacterColor(c, r);
                                    change.OldOpacity = surface.IsCellOpaque(c, r);
                                    change.OldSpecialInfo = surface.GetSpecialInfo(c, r);

                                    change.NewCharacter = change.OldCharacter;
                                    change.NewBackgroundColor = change.OldBackgroundColor;
                                    change.NewCharacterColor = change.OldCharacterColor;
                                    change.NewOpacity = change.OldOpacity;
                                    change.NewSpecialInfo = change.OldSpecialInfo;

                                    if (c + i < surface.Width && r + j < surface.Height)
                                    {
                                        change.NewOpacity = !surface.IsCellOpaque(x, y);
                                        surface.SetCellOpacity(c + i, r + j, !surface.IsCellOpaque(c + i, r + j));
                                    }

                                    changes.Add(change);
                                }
                            }
                            break;

                        case InputMode.SmallText:
                            ShortTextForm form = new ShortTextForm();
                            DialogResult result = form.ShowDialog(this);

                            if (result == DialogResult.OK)
                            {
                                surface.BlitString(changes, form.ChosenText, (Parent as EditorForm).BrushControl.CharacterColor, c, r);
                                Refresh();
                            }
                            break;
                        case InputMode.LongText:
                            LongTextForm longTextForm = new LongTextForm();
                            DialogResult longResult = longTextForm.ShowDialog(this);

                            if (longResult == DialogResult.OK)
                            {
                                surface.BlitStringMultiline(changes, longTextForm.ChosenText, (Parent as EditorForm).BrushControl.CharacterColor,
                                    c, r, selectionSize.X, selectionSize.Y);
                                Refresh();
                            }
                            break;
                        case InputMode.FillCells:
                            EditorForm parent = (Parent as EditorForm);
                            BrushControl brushControl = parent.BrushControl;

                            if (brushControl.PaintCharacter)
                            {
                                FloodFillChar(changes, c, r, brushControl.Character);
                            }
                            if (brushControl.PaintBackgroundColor)
                            {
                                FloodFillBack(changes, c, r, brushControl.BackgroundColor);
                            }
                            if (brushControl.PaintCharacterColor)
                            {
                                FloodFillFore(changes, c, r, brushControl.CharacterColor);
                            }

                            Refresh();
                            break;
                        case InputMode.EyeDropper:
                            parent = (Parent as EditorForm);
                            brushControl = parent.BrushControl;

                            if (brushControl.PaintCharacter)
                            {
                                brushControl.Character = surface.GetCharacter(c, r);
                            }
                            if (brushControl.PaintBackgroundColor)
                            {
                                brushControl.BackgroundColor = surface.GetBackgroundColor(c, r);
                            }
                            if (brushControl.PaintCharacterColor)
                            {
                                brushControl.CharacterColor = surface.GetCharacterColor(c, r);
                            }
                            break;
                        case InputMode.Shapes:
                            firstCorner = selectedCell;
                            break;
                    }

                    AddAction(changes);
                }
            }
            RefreshRect(new Rectangle(selectedCell.X, selectedCell.Y, selectionSize.X, selectionSize.Y));
            DrawCursor(new Point(selectedCell.X, selectedCell.Y));
        }

        private void SurfacePanel_MouseUp(object sender, MouseEventArgs e)
        {
            //handle one-time mouse release inputs
            if (e.Button == MouseButtons.Left)
            {
                if (surface != null && surface.IsInBounds(SelectedCell))
                {
                    //a cell was released over. Handle the appropriate behavior for the selected mode.
                    switch (Mode)
                    {
                        case InputMode.Shapes:
                            ShapesControl.ShapeType type = (Parent as EditorForm).ShapesControl.ChosenType;
                            BrushControl brush = (Parent as EditorForm).BrushControl;

                            Rectangle rect = new Rectangle();
                            rect.X = Math.Min(selectedCell.X, firstCorner.X);
                            rect.Y = Math.Min(selectedCell.Y, firstCorner.Y);
                            rect.Width = Math.Abs(selectedCell.X - firstCorner.X) + 1;
                            rect.Height = Math.Abs(selectedCell.Y - firstCorner.Y) + 1;

                            if (type == ShapesControl.ShapeType.Line)
                            {
                                surface.DrawLine(selectedCell, firstCorner, brush.PaintCharacter, brush.PaintBackgroundColor, 
                                    brush.PaintCharacterColor, brush.Character, brush.BackgroundColor, brush.CharacterColor);
                            }
                            else
                            {
                                if (type == ShapesControl.ShapeType.EmptyRectangle)
                                {
                                    surface.DrawRect(rect, brush.PaintCharacter, brush.PaintBackgroundColor,
                                        brush.PaintCharacterColor, brush.Character, brush.BackgroundColor, brush.CharacterColor);
                                }
                                if (type == ShapesControl.ShapeType.FilledRectangle)
                                {
                                    surface.FillRect(rect, brush.PaintCharacter, brush.PaintBackgroundColor,
                                        brush.PaintCharacterColor, brush.Character, brush.BackgroundColor, brush.CharacterColor);
                                }
                            }

                            RefreshRect(rect);
                            DrawCursor(selectedCell);
                            firstCorner = new Point(-1, -1);

                            break;
                    }
                }
            }
        }

        #endregion

        #region Surface Editing Methods

        public void ClearTransparency()
        {
            //add cell changes to stack
            List<CellChange> changes = new List<CellChange>();

            for (int x = 0; x < surface.Width; ++x)
            {
                for (int y = 0; y < surface.Height; ++y)
                {
                    CellChange change = new CellChange();
                    change.X = x;
                    change.Y = y;

                    change.OldCharacter = surface.GetCharacter(x, y);
                    change.OldBackgroundColor = surface.GetBackgroundColor(x, y);
                    change.OldCharacterColor = surface.GetCharacterColor(x, y);
                    change.OldOpacity = surface.IsCellOpaque(x, y);
                    change.OldSpecialInfo = surface.GetSpecialInfo(x, y);

                    change.NewCharacter = change.OldCharacter;
                    change.NewBackgroundColor = change.OldBackgroundColor;
                    change.NewCharacterColor = change.OldCharacterColor;
                    change.NewOpacity = true;
                    change.NewSpecialInfo = change.OldSpecialInfo;

                    changes.Add(change);
                }
            }

            AddAction(changes); 

            surface.ClearTransparency();
        }

        #endregion

        #region Undo/Redo Methods

        void AddAction(List<CellChange> changes)
        {
            if (changes.Count() > 0)
            {
                actions.Push(changes);
                undoneActions.Clear();
            }
        }

        public void Undo()
        {
            if (actions.Count() > 0)
            {
                List<CellChange> changes = actions.Pop();
                surface.UndoChanges(changes);
                undoneActions.Push(changes);

                Refresh();
            }
        }

        public void Redo()
        {
            if (undoneActions.Count() > 0)
            {
                List<CellChange> changes = undoneActions.Pop();
                surface.RedoChanges(changes);
                actions.Push(changes);

                Refresh();
            }
        }

        #endregion

        #region Flood Fill Helpers

        private void FloodFillChar(List<CellChange> changes, int x, int y, char value)
        {
            char prevValue = surface.GetCharacter(x, y);

            if (prevValue == value)
                return;


            CellChange change = new CellChange();
            change.X = x;
            change.Y = y;
            int c = x;
            int r = y;
            change.OldCharacter = surface.GetCharacter(c, r);
            change.OldBackgroundColor = surface.GetBackgroundColor(c, r);
            change.OldCharacterColor = surface.GetCharacterColor(c, r);
            change.OldOpacity = surface.IsCellOpaque(c, r);
            change.OldSpecialInfo = surface.GetSpecialInfo(c, r);

            change.NewCharacter = value;
            change.NewBackgroundColor = change.OldBackgroundColor;
            change.NewCharacterColor = change.OldCharacterColor;
            change.NewOpacity = change.OldOpacity;
            change.NewSpecialInfo = change.OldSpecialInfo;

            changes.Add(change);

            surface.SetCharacter(x, y, value);

            if (x > 0 && surface.GetCharacter(x - 1, y) == prevValue)
            {
                FloodFillChar(changes, x - 1, y, value);
            }

            if (y > 0 && surface.GetCharacter(x, y - 1) == prevValue)
            {
                FloodFillChar(changes, x, y - 1, value);
            }

            if (x < surface.Width - 1 && surface.GetCharacter(x + 1, y) == prevValue)
            {
                FloodFillChar(changes, x + 1, y, value);
            }

            if (y < surface.Height - 1 && surface.GetCharacter(x, y + 1) == prevValue)
            {
                FloodFillChar(changes, x, y + 1, value);
            }
        }

        private void FloodFillBack(List<CellChange> changes, int x, int y, Color value)
        {
            Color prevValue = surface.GetBackgroundColor(x, y);

            if (prevValue.R == value.R && prevValue.G == value.G && prevValue.B == value.B)
                return;

            CellChange change = new CellChange();
            change.X = x;
            change.Y = y;
            int c = x;
            int r = y;
            change.OldCharacter = surface.GetCharacter(c, r);
            change.OldBackgroundColor = surface.GetBackgroundColor(c, r);
            change.OldCharacterColor = surface.GetCharacterColor(c, r);
            change.OldOpacity = surface.IsCellOpaque(c, r);
            change.OldSpecialInfo = surface.GetSpecialInfo(c, r);

            change.NewCharacter = change.OldCharacter;
            change.NewBackgroundColor = value;
            change.NewCharacterColor = change.OldCharacterColor;
            change.NewOpacity = change.OldOpacity;
            change.NewSpecialInfo = change.OldSpecialInfo;

            changes.Add(change);

            surface.SetBackgroundColor(x, y, value);

            if (x > 0 && surface.GetBackgroundColor(x - 1, y) == prevValue)
            {
                FloodFillBack(changes, x - 1, y, value);
            }

            if (y > 0 && surface.GetBackgroundColor(x, y - 1) == prevValue)
            {
                FloodFillBack(changes, x, y - 1, value);
            }

            if (x < surface.Width - 1 && surface.GetBackgroundColor(x + 1, y) == prevValue)
            {
                FloodFillBack(changes, x + 1, y, value);
            }

            if (y < surface.Height - 1 && surface.GetBackgroundColor(x, y + 1) == prevValue)
            {
                FloodFillBack(changes, x, y + 1, value);
            }
        }

        private void FloodFillFore(List<CellChange> changes, int x, int y, Color value)
        {
            Color prevValue = surface.GetCharacterColor(x, y);

            if (prevValue.R == value.R && prevValue.G == value.G && prevValue.B == value.B)
                return;

            CellChange change = new CellChange();
            change.X = x;
            change.Y = y;
            int c = x;
            int r = y;
            change.OldCharacter = surface.GetCharacter(c, r);
            change.OldBackgroundColor = surface.GetBackgroundColor(c, r);
            change.OldCharacterColor = surface.GetCharacterColor(c, r);
            change.OldOpacity = surface.IsCellOpaque(c, r);
            change.OldSpecialInfo = surface.GetSpecialInfo(c, r);

            change.NewCharacter = change.OldCharacter;
            change.NewBackgroundColor = change.OldBackgroundColor;
            change.NewCharacterColor = value;
            change.NewOpacity = change.OldOpacity;
            change.NewSpecialInfo = change.OldSpecialInfo;

            changes.Add(change);

            surface.SetCharacterColor(x, y, value);

            if (x > 0 && surface.GetCharacterColor(x - 1, y) == prevValue)
            {
                FloodFillFore(changes, x - 1, y, value);
            }

            if (y > 0 && surface.GetCharacterColor(x, y - 1) == prevValue)
            {
                FloodFillFore(changes, x, y - 1, value);
            }

            if (x < surface.Width - 1 && surface.GetCharacterColor(x + 1, y) == prevValue)
            {
                FloodFillFore(changes, x + 1, y, value);
            }

            if (y < surface.Height - 1 && surface.GetCharacterColor(x, y + 1) == prevValue)
            {
                FloodFillFore(changes, x, y + 1, value);
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

            graphics.FillRectangle(brush, 
                new Rectangle(rectangle.X - hScrollBar1.Value, rectangle.Y - vScrollBar1.Value, rectangle.Width, rectangle.Height));

            brush.Dispose();
        }

        private void DrawCursor(Point position)
        {
            if (surface == null) return;

            if (Mode == InputMode.Shapes && firstCorner != new Point(-1, -1))
            {
                int x1 = Math.Min(firstCorner.X, selectedCell.X);
                int y1 = Math.Min(firstCorner.Y, selectedCell.Y);
                int w1 = Math.Abs(firstCorner.X - selectedCell.X);
                int h1 = Math.Abs(firstCorner.Y - selectedCell.Y);

                FillRectangle(new Rectangle(x1 * CHAR_WIDTH, y1 * CHAR_HEIGHT, CHAR_WIDTH * w1, CHAR_HEIGHT * h1), CURSOR_COLOR);

                return;
            }

            int x = position.X * CHAR_WIDTH;
            int y = position.Y * CHAR_HEIGHT;

            int w = Math.Min(selectionSize.X, surface.Width - position.X);
            int h = Math.Min(selectionSize.Y, surface.Height - position.Y);

            FillRectangle(new Rectangle(x, y, CHAR_WIDTH * w, CHAR_HEIGHT * h), CURSOR_COLOR);
        }

        public void RefreshRect(Rectangle rect)
        {
            //draw editor background color
            Rectangle backRect = new Rectangle(rect.X * CHAR_WIDTH, rect.Y * CHAR_HEIGHT, rect.Width * CHAR_WIDTH, rect.Height * CHAR_HEIGHT);
            FillRectangle(backRect, BACKGROUND_COLOR);

            if (surface == null) return;

            //draw all background colors
            for (int y = rect.Y; y < rect.Bottom && y < surface.Height; ++y)
            {
                if (y < 0) continue;

                int x = rect.X;

                while (x < rect.Right && x < surface.Width)
                {
                    if (x < 0)
                    {
                        ++x;
                        continue;
                    }

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
                if (y < 0) continue;

                int x = rect.X;

                while (x < rect.Right && x < surface.Width)
                {
                    if (x < 0)
                    {
                        ++x;
                        continue;
                    }

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

                    graphics.DrawString(text, font, new SolidBrush(characterColor), new PointF(destx + CHAR_OFFSET - hScrollBar1.Value, desty - vScrollBar1.Value));
                }
            }

            //draw special info overlays
            if (Mode == InputMode.SpecialInfo)
            {
                for (int c = rect.X; c < rect.Right && c < surface.Width; ++c)
                {
                    for (int r = rect.Y; r < rect.Bottom && r < surface.Height; ++r)
                    {
                        if (c >= 0 && r >= 0)
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
        }

        #endregion

        #region Scrollbar/Resize Events and Helpers

        public void UpdateScrollBars()
        {
            hScrollBar1.Visible = false;
            vScrollBar1.Visible = false;

            if (surface.PixelWidth > this.Width)
            {
                hScrollBar1.Visible = true;
                hScrollBar1.Maximum = surface.PixelWidth - this.Width;
            }

            if (surface.PixelHeight > this.Height)
            {
                vScrollBar1.Visible = true;
                vScrollBar1.Maximum = surface.PixelHeight - this.Height;
            }
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            if (e.Type == ScrollEventType.EndScroll)
            {
                Refresh();
            }
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            if (e.Type == ScrollEventType.EndScroll)
            {
                Refresh();
            }
        }

        private void SurfacePanel_Resize(object sender, EventArgs e)
        {
            if (surface != null)
            {
                UpdateScrollBars();
            }

            if (graphics != null)
            {
                //make a new graphics object
                graphics.Dispose();
                graphics = this.CreateGraphics();
            }
        }

        #endregion
    }
}
