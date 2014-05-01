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
    public partial class BrushControl : UserControl
    {
        public BrushControl()
        {
            InitializeComponent();
        }

        private void BrushControl_Load(object sender, EventArgs e)
        {
            bgColorPicker.Color = Color.Black;
        }
    }
}
