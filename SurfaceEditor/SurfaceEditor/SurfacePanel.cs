using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SurfaceEditor.Lib;
using System.Drawing.Imaging;
using System.IO;

namespace SurfaceEditor
{
    /// <summary>
    /// Panel which displays the surface being edited.
    /// </summary>
    public partial class SurfacePanel : UserControl
    {
        public const int CHAR_WIDTH = 8;
        public const int CHAR_HEIGHT = 12;

        Graphics graphics;
        Font font;
        Surface surface;

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

                string directoryPath = Path.GetDirectoryName(surface.FilePath);

	            //draw all images
                foreach (KeyValuePair<Point, string> pair in surface.Images)
                {
                    Image bmp = Bitmap.FromFile(directoryPath + "/" + surface.ImagePaths[pair.Value]);

                    ImageAttributes attr = new ImageAttributes();
                    attr.SetColorKey(surface.ImageColorKeys[pair.Value], surface.ImageColorKeys[pair.Value]); 

                    Rectangle dstRect = new Rectangle(pair.Key.X * CHAR_WIDTH, pair.Key.Y * CHAR_HEIGHT, bmp.Width, bmp.Height);
                    graphics.DrawImage(bmp, dstRect, 0, 0, bmp.Width, bmp.Height,
                        GraphicsUnit.Pixel, attr);
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

        #region Graphics Helper Methods

        private void Clear()
        {
            graphics.Clear(Color.Gray);
        }

        private void FillRectangle(Rectangle rectangle, Color color)
        {
            Brush brush = new SolidBrush(color);

            graphics.FillRectangle(brush, rectangle);

            brush.Dispose();
        }

        #endregion
    }
}
