using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SurfaceEditor.Lib
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

        #region IO

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

        public void WriteToFile(string path)
        {

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

        #endregion

        #region Draw Methods

        /// <summary>
        /// Fills a rectangle on the Surface with the given cell info.
        /// </summary>
        public void FillRect(Rectangle dest, char character, Color backgroundColor, Color characterColor)
        {
            for (int x = dest.X; x < dest.Right; ++x)
            {
                for (int y = dest.Y; y < dest.Bottom; ++y)
                {
                    SetCharacter(x, y, character);
                    SetBackgroundColor(x, y, backgroundColor);
                    SetCharacterColor(x, y, characterColor);
                }
            }
        }

        /// <summary>
        /// Draws a hollow rectangle on the Surface with the given cell info.
        /// </summary>
        public void DrawRect(Rectangle dest, char character, Color backgroundColor, Color characterColor)
        {
            int x1 = dest.Left;
            int x2 = dest.Right - 1;

            int y1 = dest.Top;
            int y2 = dest.Bottom - 1;

            //draw the top and bottom
            for (int x = x1; x <= x2; ++x)
            {
                SetCharacter(x, y1, character);
                SetBackgroundColor(x, y1, backgroundColor);
                SetCharacterColor(x, y1, characterColor);

                SetCharacter(x, y2, character);
                SetBackgroundColor(x, y2, backgroundColor);
                SetCharacterColor(x, y2, characterColor);
            }

            //draw the left and right
            for (int y = y1; y <= y2; ++y)
            {
                SetCharacter(x1, y, character);
                SetBackgroundColor(x1, y, backgroundColor);
                SetCharacterColor(x1, y, characterColor);

                SetCharacter(x1, y, character);
                SetBackgroundColor(x1, y, backgroundColor);
                SetCharacterColor(x1, y, characterColor);
            }
        }

        #endregion

        #region Blit Methods

        public void BlitString(string text, Color color, int x, int y)
        {
            for (int i = 0; i < text.Length; ++i)
            {
                SetCharacter(x + i, y, text[i]);
                SetCharacterColor(x + i, y, color);
            }
        }

        public void BlitStringMultiline(string text, Color color, int x, int y, int maxWidth)
        {
            string[] words = text.Split(' ');

            Rectangle dest = new Rectangle(x, y, maxWidth, 0);

            foreach (string word in words)
            {
                if (x - dest.Left + word.Length > maxWidth)
                {
                    //wrap to the next line
                    x = dest.Left;
                    ++y;
                }

                BlitString(word, color, x, y);

                x += word.Length + 1; //move over for the next word
            }
        }

        #endregion

        #region Helpers

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

        #endregion
    }
}
