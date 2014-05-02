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

        #endregion

        #region File Menu Events

        private void openSurfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                surfacePanel1.Surface = Surface.FromFile(dialog.FileName);
                surfacePanel1.Refresh();
            }

            cellInfo1.Surface = surfacePanel1.Surface;
            cellInfo1.Enabled = true;
            
            toolbox1.Enabled = true;
        }

        #endregion

        private void surfacePanel1_MouseMove(object sender, MouseEventArgs e)
        {
            Point cell = new Point(e.X / SurfacePanel.CHAR_WIDTH, e.Y / SurfacePanel.CHAR_HEIGHT);

            if (cellInfo1.Surface != null && surfacePanel1.Surface.IsInBounds(cell))
            {
                cellInfo1.ShowCellInfo(cell.X, cell.Y);
            }
            else
            {
                cellInfo1.ClearCellInfo();
            }

            if (surfacePanel1.Surface != null && cell != surfacePanel1.SelectedCell)
            {
                surfacePanel1.SelectedCell = cell;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            surfacePanel1.Surface.ClearTransparency();
            surfacePanel1.Refresh();
        }
    }
}
