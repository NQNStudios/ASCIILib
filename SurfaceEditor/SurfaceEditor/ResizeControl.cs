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
    public partial class ResizeControl : UserControl
    {
        public ResizeControl()
        {
            InitializeComponent();
        }

        public void Init(string text, int width, int height, int maxWidth, int maxHeight)
        {
            groupBox1.Text = text;

            widthControl.Maximum = maxWidth;
            heightControl.Maximum = maxHeight;

            widthControl.Value = width;
            heightControl.Value = height;
        }

        public NumericUpDown WidthControl
        {
            get { return widthControl; }
        }

        public NumericUpDown HeightControl
        {
            get { return heightControl; }
        }
    }
}
