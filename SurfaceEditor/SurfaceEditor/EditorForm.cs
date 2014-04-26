using SurfaceEditor.Lib;
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
    /// <summary>
    /// The main form for the Surface Editor.
    /// </summary>
    public partial class EditorForm : Form
    {
        public EditorForm()
        {
            InitializeComponent();
        }

        private void openSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                surfacePanel1.Surface = Surface.FromFile(dialog.FileName);
                surfacePanel1.Refresh();
            }
        }
    }
}
