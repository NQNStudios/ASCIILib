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
    public partial class ShiftControl : UserControl
    {
        Surface surface;
        SurfacePanel surfacePanel;

        public ShiftControl()
        {
            InitializeComponent();
        }

        private void ShiftControl_Load(object sender, EventArgs e)
        {
            surfacePanel = (Parent as EditorForm).SurfacePanel;
            surface = surfacePanel.Surface;
        }

        private void upButton_Click(object sender, EventArgs e)
        {
            surface.ShiftUp();
            surfacePanel.Refresh();
        }

        private void leftButton_Click(object sender, EventArgs e)
        {
            surface.ShiftLeft();
            surfacePanel.Refresh();
        }

        private void rightButton_Click(object sender, EventArgs e)
        {
            surface.ShiftRight();
            surfacePanel.Refresh();
        }

        private void downButton_Click(object sender, EventArgs e)
        {
            surface.ShiftDown();
            surfacePanel.Refresh();
        }
    }
}
