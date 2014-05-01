using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SurfaceEditor
{
    public partial class CellInfo : UserControl
    {
        public CellInfo()
        {
            InitializeComponent();
        }

        public Surface Surface
        {
            get;
            set;
        }

        public void ShowCellInfo(int x, int y)
        {
            charTextBox.Text = "" + Surface.GetCharacter(x, y);
            
            charColorTextBox.Text = ColorToString(Surface.GetCharacterColor(x, y));
            charColorTextBox.BackColor = Surface.GetCharacterColor(x, y);

            backColorTextBox.Text = ColorToString(Surface.GetBackgroundColor(x, y));
            backColorTextBox.BackColor = Surface.GetBackgroundColor(x, y);

            opacityTextBox.Text = Surface.IsCellOpaque(x, y) ? "True" : "False";
            infoTextBox.Text = Surface.GetSpecialInfo(x, y);
        }

        public void ClearCellInfo()
        {
            charTextBox.Text = "";

            charColorTextBox.Text = "";
            charColorTextBox.BackColor = Color.White;

            backColorTextBox.Text = "";
            backColorTextBox.BackColor = Color.White;

            opacityTextBox.Text = "";
            infoTextBox.Text = "";
        }

        private string ColorToString(Color color)
        {
            return "(" + color.R + ", " + color.G + ", " + color.B + ")";
        }
    }
}
