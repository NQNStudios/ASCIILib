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

        #region Properties

        public CellInfo CellInfo
        {
            get { return cellInfo1; }
        }

        public SurfacePanel SurfacePanel
        {
            get { return surfacePanel1; }
        }

        public ResizeControl ResizeControl
        {
            get { return resizeControl1; }
        }

        public BrushControl BrushControl
        {
            get { return brushControl1; }
        }

        public Button ClearTransparencyButton
        {
            get { return button1; }
        }

        public ShiftControl ShiftControl
        {
            get { return shiftControl1; }
        }

        public SpecialInfoControl SpecialInfoControl
        {
            get { return specialInfoControl1; }
        }

        #endregion

        #region File Menu Events

        private void openSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                SetSurface(Surface.FromFile(dialog.FileName));
            }
        }

        private void SetSurface(Surface surface)
        {
            surfacePanel1.Surface = surface;
            surfacePanel1.Refresh();

            //prepare the editor now that a new surface has been loaded.

            cellInfo1.Surface = surfacePanel1.Surface;
            cellInfo1.Enabled = true;

            toolbox1.Enabled = true;

            SpecialInfoControl.ClearLabels();
            SpecialInfoControl.LoadLabels(surface);
        }

        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            surfacePanel1.Surface.ClearTransparency();
            surfacePanel1.Refresh();
        }

        private void newSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewSurfaceForm dialog = new NewSurfaceForm();

            DialogResult result = dialog.ShowDialog(this);

            if (result == DialogResult.OK)
            {
                SetSurface(new Surface(dialog.SurfaceWidth, dialog.SurfaceHeight));
            }
        }
    }
}
