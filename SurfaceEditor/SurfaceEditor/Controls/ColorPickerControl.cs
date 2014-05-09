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
    public partial class ColorPickerControl : UserControl
    {
        Color color;

        #region Initialization

        public ColorPickerControl()
        {
            InitializeComponent();
        }

        private void ColorPickerControl_Load(object sender, EventArgs e)
        {
            Color = Color.White;
        }

        #endregion

        #region Properties

        public Color Color
        {
            get { return color; }
            set 
            {
                color = value;
                this.BackColor = value;
            }
        }

        #endregion

        #region Events

        private void ColorPickerControl_Click(object sender, EventArgs e)
        {
            if (Enabled)
            {
                DialogResult result = colorDialog.ShowDialog();

                if (result == DialogResult.OK)
                {
                    Color = colorDialog.Color;
                }
            }
        }

        #endregion
    }
}
