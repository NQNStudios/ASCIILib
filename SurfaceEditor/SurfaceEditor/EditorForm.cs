using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
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

        string openFile = "";

        #region Properties

        public Label CursorLabel
        {
            get { return cursorLabel; }
        }

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

        public ShapesControl ShapesControl
        {
            get { return shapesControl1; }
        }

        #endregion

        #region File Menu Events

        private void openSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Surface File | *.srf";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                openFile = dialog.FileName;
                SetSurface(Surface.FromFile(dialog.FileName));
            }
        }

        private void newSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewSurfaceForm dialog = new NewSurfaceForm();

            DialogResult result = dialog.ShowDialog(this);

            if (result == DialogResult.OK)
            {
                SetSurface(new Surface(dialog.SurfaceWidth, dialog.SurfaceHeight));
            }

            openFile = "";
        }

        private void saveSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (surfacePanel1.Surface != null)
            {
                SaveFileDialog dialog = new SaveFileDialog();
                if (openFile.Length > 0)
                {
                    dialog.InitialDirectory = Path.GetDirectoryName(openFile);
                    dialog.FileName = Path.GetFileName(openFile);
                }
                dialog.Filter = "Surface File | *.srf";

                DialogResult result = dialog.ShowDialog(this);

                if (result == DialogResult.OK)
                {
                    string path = dialog.FileName;

                    surfacePanel1.Surface.SaveToFile(path);

                    openFile = path;
                }
            }
        }

        private void SetSurface(Surface surface)
        {
            surfacePanel1.Surface = surface;
            surfacePanel1.UpdateScrollBars();
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
            surfacePanel1.ClearTransparency();
            surfacePanel1.Refresh();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            surfacePanel1.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            surfacePanel1.Redo();
        }
    }
}
