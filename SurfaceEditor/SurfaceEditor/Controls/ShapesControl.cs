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
    public partial class ShapesControl : UserControl
    {
        public enum ShapeType
        {
            Line,
            EmptyRectangle,
            FilledRectangle
        }

        public ShapesControl()
        {
            InitializeComponent();
        }

        public ShapeType ChosenType
        {
            get
            {
                if (lineButton.Checked) return ShapeType.Line;

                if (filledCheckBox.Checked) return ShapeType.FilledRectangle;

                return ShapeType.EmptyRectangle;
            }
        }

        private void rectangleButton_CheckedChanged(object sender, EventArgs e)
        {
            if (rectangleButton.Checked)
            {
                filledCheckBox.Visible = true;
            }
            else
            {
                filledCheckBox.Visible = false;
            }
        }

        private void lineButton_CheckedChanged(object sender, EventArgs e)
        {
            if (lineButton.Checked)
            {
            }
            else
            {
            }
        }
    }
}
