using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SurfaceEditor
{
    public partial class NewSurfaceForm : Form
    {
        public NewSurfaceForm()
        {
            InitializeComponent();
        }

        public int SurfaceWidth
        {
            get { return (int)resizeControl1.WidthControl.Value; }
        }

        public int SurfaceHeight
        {
            get { return (int)resizeControl1.HeightControl.Value; }
        }

        private void resizeControl1_Load(object sender, EventArgs e)
        {
            resizeControl1.Init("Surface Size", 80, 25, Toolbox.MAX_SURFACE_WIDTH, Toolbox.MAX_SURFACE_HEIGHT);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
