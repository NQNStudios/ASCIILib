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
    /// <summary>
    /// Panel which displays the surface being edited.
    /// </summary>
    public partial class SurfacePanel : UserControl
    {
        private Graphics graphics;

        public SurfacePanel()
        {
            InitializeComponent();
        }

        private void SurfacePanel_Load(object sender, EventArgs e)
        {
            graphics = this.CreateGraphics();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Clear();
        }

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
