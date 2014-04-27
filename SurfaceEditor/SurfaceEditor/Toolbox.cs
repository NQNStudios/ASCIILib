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
    public partial class Toolbox : UserControl
    {
        public Toolbox()
        {
            InitializeComponent();
        }

        void viewCellsButton_CheckedChanged(object sender, EventArgs e)
        {
            (Parent as EditorForm).CellInfo.Visible = viewCellsButton.Checked;
        }

        private void paintCellsButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void toggleOpacityButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void placeInfoButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void imagesButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void longTextButton_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
