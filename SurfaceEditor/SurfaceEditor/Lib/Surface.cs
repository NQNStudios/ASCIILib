using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SurfaceEditor
{
    /// <summary>
    /// Represents an ASCIILib surface in the editor. This class is different from the ASCIILib Surface class because Surfaces in the editor can be resized.
    /// </summary>
    public class Surface
    {
        #region Default Cell Constants

        public const char DEFAULT_CHAR = ' ';
        public Color DEFAULT_CHAR_COLOR = Color.White;
        public Color DEFAULT_BACK_COLOR = Color.Black;
        public const bool DEFAULT_CELL_OPACITY = true;
        public const string DEFAULT_SPECIAL_INFO = "";

        #endregion

        #region Fields

        int width, height;

        List<List<char>> characters = new List<List<char>>();
        List<List<Color>> characterColors = new List<List<Color>>();
        List<List<Color>> backgroundColors = new List<List<Color>>();

        List<List<bool>> cellOpacity = new List<List<bool>>();
        List<List<string>> specialInfo = new List<List<string>>();

        Dictionary<string, string> imagePaths = new Dictionary<string, string>();
        Dictionary<string, Color> imageColorKeys = new Dictionary<string, Color>();
        Dictionary<Point, string> images = new Dictionary<Point, string>();

        #endregion

        #region Initialization

        public Surface(int width, int height)
        {
            SetWidth(width);
            SetHeight(height);
        }

        public Surface()
            : this(0, 0)
        {
        }

        #endregion

        #region File IO

        public static Surface FromFile(string path)
        {
            Surface surface = new Surface();

            surface.FilePath = path;

            StreamReader reader = new StreamReader(path);
            string line = "";
            
            //COLORS
            line = reader.ReadLine();
            line = reader.ReadLine();

            Dictionary<char, Color> colors = new Dictionary<char, Color>();

            do
            {
                string[] split = line.Split(' ');

                char key = split[0][0];

                string r = split[1];
                string g = split[2];
                string b = split[3];

                colors[key] = Color.FromArgb(int.Parse(r), int.Parse(g), int.Parse(b));
            } while ((line = reader.ReadLine()) != "INFO CODES"); //Do-while loop used because COLORS will never be an empty section

            Dictionary<char, string> infoCodes = new Dictionary<char, string>();

            //INFO CODES
            while ((line = reader.ReadLine()) != "SIZE") //While loop used here because INFO CODES may be an empty section
            {
                string[] split = line.Split(' ');

                char key = split[0][0];
                string code = split[1];

                if (code == ".") code = "";

                infoCodes[key] = code;
            }

            //SIZE
            line = reader.ReadLine();

            string[] dimensions = line.Split(' ');

            int width = int.Parse(dimensions[0]);
            int height = int.Parse(dimensions[1]);

            surface.SetWidth(width);
            surface.SetHeight(height);

            //CHARACTERS
            line = reader.ReadLine();

            for (int r = 0; r < height; ++r) //for loop used because this section will have fixed size
            {
                line = reader.ReadLine();

                for (int c = 0; c < width; ++c)
                {
                    char character = line[c];

                    surface.SetCharacter(c, r, character);
                }
            }

            //BACKGROUND COLORS
            line = reader.ReadLine();

            for (int r = 0; r < height; ++r) //for loop used because this section will have fixed size
            {
                line = reader.ReadLine();

                for (int c = 0; c < width; ++c)
                {
                    char key = line[c];

                    surface.SetBackgroundColor(c, r, colors[key]);
                }
            }

            //CHARACTER COLORS
            line = reader.ReadLine();

            for (int r = 0; r < height; ++r) //for loop used because this section will have fixed size
            {
                line = reader.ReadLine();

                for (int c = 0; c < width; ++c)
                {
                    char key = line[c];

                    surface.SetCharacterColor(c, r, colors[key]);
                }
            }

            //OPACITY
            line = reader.ReadLine();

            for (int r = 0; r < height; ++r) //for loop used because this section will have fixed size
            {
                line = reader.ReadLine();

                for (int c = 0; c < width; ++c)
                {
                    char opacity = line[c];

                    surface.SetCellOpacity(c, r, opacity == '1');
                }
            }

            //SPECIAL INFO
            line = reader.ReadLine();

            for (int r = 0; r < height; ++r) //for loop used because this section will have fixed size
            {
                line = reader.ReadLine();

                for (int c = 0; c < width; ++c)
                {
                    char key = line[c];

                    if (key != ' ')
                    {
                        surface.SetSpecialInfo(c, r, infoCodes[key]);
                    }
                }
            }

            reader.Close();

            return surface;
        }

        public void SaveToFile(string path)
        {
            StreamWriter writer = new StreamWriter(path);

            writer.WriteLine("COLORS");

            Dictionary<Color, char> colorKeys = new Dictionary<Color, char>();
            char key = (char)33;

            for (int x = 0; x < width; ++x)
            {
                for (int y = 0; y < height; ++y)
                {
                    Color bgCol = GetBackgroundColor(x, y);
                    Color fgCol = GetCharacterColor(x, y);

                    if (!colorKeys.Keys.Contains(bgCol))
                    {
                        colorKeys[bgCol] = key++;
                    }

                    if (!colorKeys.Keys.Contains(fgCol))
                    {
                        colorKeys[fgCol] = key++;
                    }
                }
            }

            foreach (KeyValuePair<Color, char> pair in colorKeys)
            {
                writer.WriteLine("" + pair.Value + " " + pair.Key.R + " " + pair.Key.G + " " + pair.Key.B);
            }

            writer.WriteLine("INFO CODES");

            Dictionary<string, char> infoKeys = new Dictionary<string, char>();
            key = (char)33;

            for (int x = 0; x < width; ++x)
            {
                for (int y = 0; y < height; ++y)
                {
                    string label = GetSpecialInfo(x, y);

                    if (label == "") label = ".";

                    if (!infoKeys.Keys.Contains(label))
                    {
                        infoKeys[label] = key++;
                    }
                }
            }

            foreach (KeyValuePair<string, char> pair in infoKeys)
            {
                writer.WriteLine("" + pair.Value + " " + pair.Key);
            }

            writer.WriteLine("SIZE");

            writer.WriteLine("" + width + " " + height);

            writer.WriteLine("CHARACTERS");

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    writer.Write(GetCharacter(x, y));
                }

                writer.WriteLine();
            }

            writer.WriteLine("BACKGROUND COLORS");

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    writer.Write(colorKeys[GetBackgroundColor(x, y)]);
                }

                writer.WriteLine();
            }

            writer.WriteLine("CHARACTER COLORS");

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    writer.Write(colorKeys[GetCharacterColor(x, y)]);
                }

                writer.WriteLine();
            }

            writer.WriteLine("OPACITY");

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    char opacity = IsCellOpaque(x, y) ? '1' : '0';
                    writer.Write(opacity);
                }

                writer.WriteLine();
            }

            writer.WriteLine("SPECIAL INFO");

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    string info = GetSpecialInfo(x, y);
                    if (info == "") info = ".";
                    writer.Write(infoKeys[info]);
                }

                writer.WriteLine();
            }

            writer.Close();
        }

        #endregion

        #region Properties

        public string FilePath
        {
            get;
            set;
        }

        public int Width
        {
            get { return width; }
            set { SetWidth(value); }
        }

        public int Height
        {
            get { return height; }
            set { SetHeight(value); }
        }

        public int PixelWidth
        {
            get { return width * SurfacePanel.CHAR_WIDTH; }
        }

        public int PixelHeight
        {
            get { return height * SurfacePanel.CHAR_HEIGHT; }
        }

        public Dictionary<string, string> ImagePaths
        {
            get { return imagePaths; }
        }

        public Dictionary<string, Color> ImageColorKeys
        {
            get { return imageColorKeys; }
        }

        public Dictionary<Point, string> Images
        {
            get { return images; }
        }

        #endregion

        #region Accessors

        public char GetCharacter(int x, int y)
        {
            return characters[x][y];
        }

        public Color GetCharacterColor(int x, int y)
        {
            return characterColors[x][y];
        }

        public Color GetBackgroundColor(int x, int y)
        {
            return backgroundColors[x][y];
        }

        public bool IsCellOpaque(int x, int y)
        {
            return cellOpacity[x][y];
        }

        public string GetSpecialInfo(int x, int y)
        {
            return specialInfo[x][y];
        }

        #endregion

        #region Mutators

        public void SetCharacter(int x, int y, char value)
        {
            characters[x][y] = value;
        }

        public void SetCharacterColor(int x, int y, Color value)
        {
            characterColors[x][y] = value;
        }

        public void SetBackgroundColor(int x, int y, Color value)
        {
            backgroundColors[x][y] = value;
        }

        public void SetCellOpacity(int x, int y, bool value)
        {
            cellOpacity[x][y] = value;
        }

        public void SetSpecialInfo(int x, int y, string value)
        {
            specialInfo[x][y] = value;
        }

        #endregion

        #region Methods

        public bool IsInBounds(Point cell)
        {
            return cell.X >= 0 && cell.Y >= 0 && cell.X < Width && cell.Y < Height;
        }

        public void ClearTransparency()
        {
            for (int c = 0; c < width; ++c)
            {
                for (int r = 0; r < height; ++r)
                {
                    SetCellOpacity(c, r, true);
                }
            }
        }

        public void ShiftUp()
        {
            for (int r = 1; r < height; ++r)
            {
                for (int c = 0; c < width; ++c)
                {
                    CopyCell(c, r, c, r - 1);
                }
            }

            //Generate a new bottom row from default
            --Height;
            ++Height;
        }

        public void ShiftDown()
        {
            for (int r = height - 2; r >= 0; --r)
            {
                for (int c = 0; c < width; ++c)
                {
                    CopyCell(c, r, c, r + 1);
                }
            }

            //Reset the top row to default
            for (int c = 0; c < width; ++c)
            {
                SetCharacter(c, 0, DEFAULT_CHAR);
                SetBackgroundColor(c, 0, DEFAULT_BACK_COLOR);
                SetCharacterColor(c, 0, DEFAULT_CHAR_COLOR);
                SetCellOpacity(c, 0, DEFAULT_CELL_OPACITY);
                SetSpecialInfo(c, 0, DEFAULT_SPECIAL_INFO);
            }
        }

        public void ShiftLeft()
        {
            for (int c = 1; c < width; ++c)
            {
                for (int r = 0; r < height; ++r)
                {
                    CopyCell(c, r, c - 1, r);
                }
            }

            //Generate a new right row from default
            --Width;
            ++Width;
        }

        public void ShiftRight()
        {
            for (int c = width - 2; c >= 0; --c)
            {
                for (int r = 0; r < height; ++r)
                {
                    CopyCell(c, r, c + 1, r);
                }
            }

            //Reset the left row to default
            for (int r = 0; r < height; ++r)
            {
                SetCharacter(0, r, DEFAULT_CHAR);
                SetBackgroundColor(0, r, DEFAULT_BACK_COLOR);
                SetCharacterColor(0, r, DEFAULT_CHAR_COLOR);
                SetCellOpacity(0, r, DEFAULT_CELL_OPACITY);
                SetSpecialInfo(0, r, DEFAULT_SPECIAL_INFO);
            }
        }

        public void UndoChanges(List<CellChange> changes)
        {
            foreach (CellChange change in changes)
            {
                UndoChange(change);
            }
        }

        public void RedoChanges(List<CellChange> changes)
        {
            foreach (CellChange change in changes)
            {
                RedoChange(change);
            }
        }

        #endregion

        #region Draw Methods

        /// <summary>
        /// Fills a rectangle on the Surface with the given cell info.
        /// </summary>
        public void FillRect(Rectangle dest, bool paintChar, bool paintBgColor, bool paintCharColor,
            char character, Color backgroundColor, Color characterColor)
        {
            for (int x = dest.X; x < dest.Right; ++x)
            {
                for (int y = dest.Y; y < dest.Bottom; ++y)
                {
                    if (paintChar) SetCharacter(x, y, character);
                    if (paintBgColor) SetBackgroundColor(x, y, backgroundColor);
                    if (paintCharColor) SetCharacterColor(x, y, characterColor);
                }
            }
        }

        /// <summary>
        /// Draws a hollow rectangle on the Surface with the given cell info.
        /// </summary>
        public void DrawRect(Rectangle dest, bool paintChar, bool paintBgColor, bool paintCharColor,
            char character, Color backgroundColor, Color characterColor)
        {
            int x1 = dest.Left;
            int x2 = dest.Right - 1;

            int y1 = dest.Top;
            int y2 = dest.Bottom - 1;

            //draw the top and bottom
            for (int x = x1; x <= x2; ++x)
            {
                if (paintChar) SetCharacter(x, y1, character);
                if (paintBgColor) SetBackgroundColor(x, y1, backgroundColor);
                if (paintCharColor) SetCharacterColor(x, y1, characterColor);

                if (paintChar) SetCharacter(x, y2, character);
                if (paintBgColor) SetBackgroundColor(x, y2, backgroundColor);
                if (paintCharColor) SetCharacterColor(x, y2, characterColor);
            }

            //draw the left and right
            for (int y = y1; y <= y2; ++y)
            {
                if (paintChar) SetCharacter(x1, y, character);
                if (paintBgColor) SetBackgroundColor(x1, y, backgroundColor);
                if (paintCharColor) SetCharacterColor(x1, y, characterColor);

                if (paintChar) SetCharacter(x2, y, character);
                if (paintBgColor) SetBackgroundColor(x2, y, backgroundColor);
                if (paintCharColor) SetCharacterColor(x2, y, characterColor);
            }
        }

        public void DrawLine(Point p1, Point p2, bool paintChar, bool paintBgColor, bool paintCharColor,
            char character, Color backgroundColor, Color characterColor)
        {
            if (p2.X < p1.X)
            {
                Point temp = p2;
                p2 = p1;
                p1 = temp;
            }
            int dx = p2.X - p1.X;
            int dy = p2.Y - p1.Y;

            if (dy == 0)
            {
                dy = 1;
            }

            if (dx == 0)
            {
                //vertical line needs special handling
                FillRect(new Rectangle(p1.X, Math.Min(p1.Y, p2.Y), 1, Math.Abs(dy)), paintChar, paintBgColor, paintCharColor, character, backgroundColor, characterColor);
                return;
            }

            float slope = (float)dy / dx;

            int prevY = p1.Y;
            for (int x = p1.X; x <= p2.X; ++x)
            {
                int y = p1.Y + (int)Math.Round(slope * (x - p1.X));

                DrawLine(new Point(x, prevY), new Point(x, y), paintChar, paintBgColor, paintCharColor, character, backgroundColor, characterColor); //recurse to more vertical lines
                prevY = y;
            }
        }

        #endregion

        #region Blit Methods

        public void BlitString(List<CellChange> changes, string text, Color color, int x, int y)
        {
            for (int i = 0; i < text.Length; ++i)
            {
                if (x + i < width && y < height)
                {
                    CellChange change = new CellChange();
                    change.X = x + i;
                    change.Y = y;

                    int c = x + i;
                    int r = y;
                    change.OldCharacter = GetCharacter(c, r);
                    change.OldBackgroundColor = GetBackgroundColor(c, r);
                    change.OldCharacterColor = GetCharacterColor(c, r);
                    change.OldOpacity = IsCellOpaque(c, r);
                    change.OldSpecialInfo = GetSpecialInfo(c, r);

                    change.NewCharacter = text[i];
                    change.NewBackgroundColor = change.OldBackgroundColor;
                    change.NewCharacterColor = color;
                    change.NewOpacity = change.OldOpacity;
                    change.NewSpecialInfo = change.OldSpecialInfo;

                    changes.Add(change);

                    SetCharacter(x + i, y, text[i]);
                    SetCharacterColor(x + i, y, color);
                }
            }
        }

        public void BlitStringMultiline(List<CellChange> changes, string text, Color color, int x, int y, int maxWidth, int maxHeight)
        {
            string[] words = text.Split(' ');

            Rectangle dest = new Rectangle(x, y, maxWidth, 0);

            foreach (string word in words)
            {
                if (x - dest.Left + word.Length > maxWidth || x + word.Length > width)
                {
                    //wrap to the next line
                    x = dest.Left;
                    ++y;

                    if (y - dest.Top > maxHeight)
                    {
                        return;
                    }
                }

                BlitString(changes, word + " ", color, x, y);

                x += word.Length + 1; //move over for the next word
            }
        }

        #endregion

        #region Helpers

        void UndoChange(CellChange change)
        {
            SetCharacter(change.X, change.Y, change.OldCharacter);
            SetBackgroundColor(change.X, change.Y, change.OldBackgroundColor);
            SetCharacterColor(change.X, change.Y, change.OldCharacterColor);
            SetCellOpacity(change.X, change.Y, change.OldOpacity);
            SetSpecialInfo(change.X, change.Y, change.OldSpecialInfo);
        }

        void RedoChange(CellChange change)
        {
            SetCharacter(change.X, change.Y, change.NewCharacter);
            SetBackgroundColor(change.X, change.Y, change.NewBackgroundColor);
            SetCharacterColor(change.X, change.Y, change.NewCharacterColor);
            SetCellOpacity(change.X, change.Y, change.NewOpacity);
            SetSpecialInfo(change.X, change.Y, change.NewSpecialInfo);
        }

        void SetWidth(int value)
        {
            if (value > width)
            {
                //add more columns filled with default cells
                for (int i = 0; i < value - width; ++i)
                {
                    List<char> newCharColumn = new List<char>(height);
                    newCharColumn.AddRange(Enumerable.Repeat(DEFAULT_CHAR, height));
                    characters.Add(newCharColumn);

                    List<Color> newCharColorColumn = new List<Color>(height);
                    newCharColorColumn.AddRange(Enumerable.Repeat(DEFAULT_CHAR_COLOR, height));
                    characterColors.Add(newCharColorColumn);

                    List<Color> newBackColorColumn = new List<Color>(height);
                    newBackColorColumn.AddRange(Enumerable.Repeat(DEFAULT_BACK_COLOR, height));
                    backgroundColors.Add(newBackColorColumn);

                    List<bool> newOpacityColumn = new List<bool>(height);
                    newOpacityColumn.AddRange(Enumerable.Repeat(DEFAULT_CELL_OPACITY, height));
                    cellOpacity.Add(newOpacityColumn);

                    List<string> newSpecialInfoColumn = new List<string>(height);
                    newSpecialInfoColumn.AddRange(Enumerable.Repeat(DEFAULT_SPECIAL_INFO, height));
                    specialInfo.Add(newSpecialInfoColumn);
                }

                
            }

            else if (value < width)
            {
                //trim columns from the right edge to shrink
                for (int i = characters.Count() - 1; i >= value; --i)
                {
                    characters.RemoveAt(i);
                    characterColors.RemoveAt(i);
                    backgroundColors.RemoveAt(i);
                    cellOpacity.RemoveAt(i);
                    specialInfo.RemoveAt(i);
                }
            }

            width = value;
        }

        void SetHeight(int value)
        {
            if (value > height)
            {
                //Add more rows to the bottom filled with default cells
                for (int c = 0; c < characters.Count(); ++c)
                {
                    for (int i = 0; i < value - height; ++i)
                    {
                        characters[c].Add(DEFAULT_CHAR);
                        characterColors[c].Add(DEFAULT_CHAR_COLOR);
                        backgroundColors[c].Add(DEFAULT_BACK_COLOR);
                        cellOpacity[c].Add(DEFAULT_CELL_OPACITY);
                        specialInfo[c].Add(DEFAULT_SPECIAL_INFO);
                    }
                }
            }

            else if (value < height)
            {
                //Trim rows from the bottom edge to shrink
                for (int c = 0; c < characters.Count(); ++c)
                {
                    for (int i = characters[c].Count() - 1; i >= value; --i)
                    {
                        characters[c].RemoveAt(i);
                        characterColors[c].RemoveAt(i);
                        backgroundColors[c].RemoveAt(i);
                        cellOpacity[c].RemoveAt(i);
                        specialInfo[c].RemoveAt(i);
                    }
                }
            }

            height = value;
        }

        void CopyCell(int srcx, int srcy, int destx, int desty)
        {
            SetCharacter(destx, desty, GetCharacter(srcx, srcy));
            SetBackgroundColor(destx, desty, GetBackgroundColor(srcx, srcy));
            SetCharacterColor(destx, desty, GetCharacterColor(srcx, srcy));
            SetCellOpacity(destx, desty, IsCellOpaque(srcx, srcy));
            SetSpecialInfo(destx, desty, GetSpecialInfo(srcx, srcy));
        }

        #endregion
    }
}
