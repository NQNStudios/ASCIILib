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
    public partial class Toolbox : UserControl
    {
        public const int MAX_SURFACE_WIDTH = 500;
        public const int MAX_SURFACE_HEIGHT = 120;

        public Toolbox()
        {
            InitializeComponent();
        }

        void viewCellsButton_CheckedChanged(object sender, EventArgs e)
        {
            (Parent as EditorForm).CellInfo.Visible = viewCellsButton.Checked;
            
            if (viewCellsButton.Checked)
            {
                (Parent as EditorForm).SurfacePanel.SelectionSize = new Point(1, 1);
                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.None;
            }
        }

        private void paintCellsButton_CheckedChanged(object sender, EventArgs e)
        {
            if (paintCellsButton.Checked)
            {
                EnableBrushResize();
                EnableBrushControl();
                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.PaintBrush;
            }
            else
            {
                DisableBrushResize();
                DisableBrushControl();
            }
        }

        private void toggleOpacityButton_CheckedChanged(object sender, EventArgs e)
        {
            if (toggleOpacityButton.Checked)
            {
                EnableBrushResize();
                (Parent as EditorForm).ClearTransparencyButton.Visible = true;
                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.ToggleOpacity;
            }
            else
            {
                (Parent as EditorForm).ClearTransparencyButton.Visible = false;
                DisableBrushResize();
            }
        }

        private void placeInfoButton_CheckedChanged(object sender, EventArgs e)
        {
            if (placeInfoButton.Checked)
            {
                EnableBrushResize();
            }
            else
            {
                DisableBrushResize();
            }
        }

        private void imagesButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textButton_CheckedChanged(object sender, EventArgs e)
        {
            if (textButton.Checked)
            {
                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.SmallText;
                (Parent as EditorForm).SurfacePanel.SelectionSize = new Point(1, 1);
            }
        }

        private void longTextButton_CheckedChanged(object sender, EventArgs e)
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;

            if (longTextButton.Checked)
            {
                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.LongText;

                resize.Init("Text Bounds",
                    (Parent as EditorForm).SurfacePanel.Surface.Width / 2, (Parent as EditorForm).SurfacePanel.Surface.Height / 2,
                    MAX_SURFACE_WIDTH, MAX_SURFACE_HEIGHT);

                resize.WidthControl.ValueChanged += BrushWidthChanged;
                resize.HeightControl.ValueChanged += BrushHeightChanged;

                //trigger cursor resize
                --resize.WidthControl.Value;
                ++resize.WidthControl.Value;
                --resize.HeightControl.Value;
                ++resize.HeightControl.Value;
            }
            else
            {
                resize.WidthControl.ValueChanged -= BrushWidthChanged;
                resize.HeightControl.ValueChanged -= BrushHeightChanged;
            }

            resize.Visible = longTextButton.Checked;
        }

        private void shiftButton_CheckedChanged(object sender, EventArgs e)
        {
            if (shiftButton.Checked)
            {
                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.None;
                (Parent as EditorForm).ShiftControl.Visible = true;
            }
            else
            {
                (Parent as EditorForm).ShiftControl.Visible = false;
            }
        }

        private void resizeButton_CheckedChanged(object sender, EventArgs e)
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;

            if (resizeButton.Checked)
            {
                resize.Init("Resize Surface", 
                    (Parent as EditorForm).SurfacePanel.Surface.Width, (Parent as EditorForm).SurfacePanel.Surface.Height,
                    MAX_SURFACE_WIDTH, MAX_SURFACE_HEIGHT);

                resize.WidthControl.ValueChanged += SurfWidthChanged;
                resize.HeightControl.ValueChanged += SurfHeightChanged;

                (Parent as EditorForm).SurfacePanel.Mode = SurfacePanel.InputMode.None;
            }
            else
            {
                resize.WidthControl.ValueChanged -= SurfWidthChanged;
                resize.HeightControl.ValueChanged -= SurfHeightChanged;
            }

            resize.Visible = resizeButton.Checked;
        }

        #region Brush Resize Helpers

        private void EnableBrushResize()
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;
            SurfacePanel surfacePanel = (Parent as EditorForm).SurfacePanel;
            Surface surface = surfacePanel.Surface;

            resize.Init("Resize Brush", surfacePanel.SelectionSize.X, surfacePanel.SelectionSize.Y, surface.Width, surface.Height);

            resize.WidthControl.ValueChanged += BrushWidthChanged;
            resize.HeightControl.ValueChanged += BrushHeightChanged;

            resize.Visible = true;
        }

        private void DisableBrushResize()
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;

            resize.WidthControl.ValueChanged -= BrushWidthChanged;
            resize.HeightControl.ValueChanged -= BrushHeightChanged;

            resize.Visible = false;
        }

        private void BrushWidthChanged(object sender, EventArgs e)
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;
            SurfacePanel surfacePanel = (Parent as EditorForm).SurfacePanel;
            surfacePanel.SelectionSize = new Point((int)resize.WidthControl.Value, surfacePanel.SelectionSize.Y);
        }

        private void BrushHeightChanged(object sender, EventArgs e)
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;
            SurfacePanel surfacePanel = (Parent as EditorForm).SurfacePanel;
            surfacePanel.SelectionSize = new Point(surfacePanel.SelectionSize.X, (int)resize.HeightControl.Value);
        }

        private void SurfWidthChanged(object sender, EventArgs e)
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;
            (Parent as EditorForm).SurfacePanel.Surface.Width = (int)resize.WidthControl.Value;

            (Parent as EditorForm).SurfacePanel.Refresh();
        }

        private void SurfHeightChanged(object sender, EventArgs e)
        {
            ResizeControl resize = (Parent as EditorForm).ResizeControl;
            (Parent as EditorForm).SurfacePanel.Surface.Height = (int)resize.HeightControl.Value;

            (Parent as EditorForm).SurfacePanel.Refresh();
        }

        #endregion

        #region Brush Control Helpers

        private void EnableBrushControl()
        {
            (Parent as EditorForm).BrushControl.Visible = true;
        }

        private void DisableBrushControl()
        {
            (Parent as EditorForm).BrushControl.Visible = false;
        }

        #endregion
    }
}
