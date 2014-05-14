namespace SurfaceEditor
{
    partial class EditorForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newSurfaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openSurfaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSurfaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.button1 = new System.Windows.Forms.Button();
            this.brushControl1 = new SurfaceEditor.BrushControl();
            this.resizeControl1 = new SurfaceEditor.ResizeControl();
            this.toolbox1 = new SurfaceEditor.Toolbox();
            this.cellInfo1 = new SurfaceEditor.CellInfo();
            this.surfacePanel1 = new SurfaceEditor.SurfacePanel();
            this.shiftControl1 = new SurfaceEditor.ShiftControl();
            this.specialInfoControl1 = new SurfaceEditor.SpecialInfoControl();
            this.shapesControl1 = new SurfaceEditor.ShapesControl();
            this.cursorLabel = new System.Windows.Forms.Label();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(965, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newSurfaceToolStripMenuItem,
            this.openSurfaceToolStripMenuItem,
            this.saveSurfaceToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newSurfaceToolStripMenuItem
            // 
            this.newSurfaceToolStripMenuItem.Name = "newSurfaceToolStripMenuItem";
            this.newSurfaceToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newSurfaceToolStripMenuItem.Text = "&New Surface";
            this.newSurfaceToolStripMenuItem.Click += new System.EventHandler(this.newSurfaceToolStripMenuItem_Click);
            // 
            // openSurfaceToolStripMenuItem
            // 
            this.openSurfaceToolStripMenuItem.Name = "openSurfaceToolStripMenuItem";
            this.openSurfaceToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openSurfaceToolStripMenuItem.Text = "&Open Surface";
            this.openSurfaceToolStripMenuItem.Click += new System.EventHandler(this.openSurfaceToolStripMenuItem_Click);
            // 
            // saveSurfaceToolStripMenuItem
            // 
            this.saveSurfaceToolStripMenuItem.Name = "saveSurfaceToolStripMenuItem";
            this.saveSurfaceToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveSurfaceToolStripMenuItem.Text = "&Save Surface";
            this.saveSurfaceToolStripMenuItem.Click += new System.EventHandler(this.saveSurfaceToolStripMenuItem_Click);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(752, 391);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(110, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "Clear Transparency";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Visible = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // brushControl1
            // 
            this.brushControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.brushControl1.BackgroundColor = System.Drawing.Color.Black;
            this.brushControl1.Character = ' ';
            this.brushControl1.CharacterColor = System.Drawing.Color.White;
            this.brushControl1.Location = new System.Drawing.Point(736, 391);
            this.brushControl1.Name = "brushControl1";
            this.brushControl1.Size = new System.Drawing.Size(202, 112);
            this.brushControl1.TabIndex = 5;
            this.brushControl1.Visible = false;
            // 
            // resizeControl1
            // 
            this.resizeControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.resizeControl1.Location = new System.Drawing.Point(735, 281);
            this.resizeControl1.Name = "resizeControl1";
            this.resizeControl1.Size = new System.Drawing.Size(147, 104);
            this.resizeControl1.TabIndex = 4;
            this.resizeControl1.Visible = false;
            // 
            // toolbox1
            // 
            this.toolbox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.toolbox1.Enabled = false;
            this.toolbox1.Location = new System.Drawing.Point(736, 27);
            this.toolbox1.Name = "toolbox1";
            this.toolbox1.Size = new System.Drawing.Size(240, 248);
            this.toolbox1.TabIndex = 3;
            // 
            // cellInfo1
            // 
            this.cellInfo1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cellInfo1.BackColor = System.Drawing.SystemColors.Control;
            this.cellInfo1.Enabled = false;
            this.cellInfo1.Location = new System.Drawing.Point(735, 281);
            this.cellInfo1.Name = "cellInfo1";
            this.cellInfo1.Size = new System.Drawing.Size(209, 167);
            this.cellInfo1.Surface = null;
            this.cellInfo1.TabIndex = 2;
            // 
            // surfacePanel1
            // 
            this.surfacePanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.surfacePanel1.AutoScroll = true;
            this.surfacePanel1.Clicked = false;
            this.surfacePanel1.Location = new System.Drawing.Point(12, 27);
            this.surfacePanel1.Name = "surfacePanel1";
            this.surfacePanel1.SelectedCell = new System.Drawing.Point(0, 0);
            this.surfacePanel1.SelectionSize = new System.Drawing.Point(1, 1);
            this.surfacePanel1.Size = new System.Drawing.Size(700, 534);
            this.surfacePanel1.Surface = null;
            this.surfacePanel1.TabIndex = 0;
            // 
            // shiftControl1
            // 
            this.shiftControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.shiftControl1.Location = new System.Drawing.Point(752, 281);
            this.shiftControl1.Name = "shiftControl1";
            this.shiftControl1.Size = new System.Drawing.Size(93, 116);
            this.shiftControl1.TabIndex = 7;
            this.shiftControl1.Visible = false;
            // 
            // specialInfoControl1
            // 
            this.specialInfoControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.specialInfoControl1.Location = new System.Drawing.Point(736, 391);
            this.specialInfoControl1.Name = "specialInfoControl1";
            this.specialInfoControl1.Size = new System.Drawing.Size(227, 168);
            this.specialInfoControl1.TabIndex = 8;
            this.specialInfoControl1.Visible = false;
            // 
            // shapesControl1
            // 
            this.shapesControl1.Location = new System.Drawing.Point(736, 282);
            this.shapesControl1.Name = "shapesControl1";
            this.shapesControl1.Size = new System.Drawing.Size(202, 79);
            this.shapesControl1.TabIndex = 9;
            this.shapesControl1.Visible = false;
            // 
            // cursorLabel
            // 
            this.cursorLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cursorLabel.AutoSize = true;
            this.cursorLabel.Location = new System.Drawing.Point(13, 568);
            this.cursorLabel.Name = "cursorLabel";
            this.cursorLabel.Size = new System.Drawing.Size(0, 13);
            this.cursorLabel.TabIndex = 10;
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.undoToolStripMenuItem_Click);
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            this.redoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.redoToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.redoToolStripMenuItem.Text = "Redo";
            this.redoToolStripMenuItem.Click += new System.EventHandler(this.redoToolStripMenuItem_Click);
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(965, 587);
            this.Controls.Add(this.cursorLabel);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.brushControl1);
            this.Controls.Add(this.resizeControl1);
            this.Controls.Add(this.toolbox1);
            this.Controls.Add(this.cellInfo1);
            this.Controls.Add(this.surfacePanel1);
            this.Controls.Add(this.shiftControl1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.shapesControl1);
            this.Controls.Add(this.specialInfoControl1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "EditorForm";
            this.Text = "ASCIILib Surface Editor";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private SurfacePanel surfacePanel1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newSurfaceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openSurfaceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveSurfaceToolStripMenuItem;
        private CellInfo cellInfo1;
        private Toolbox toolbox1;
        private ResizeControl resizeControl1;
        private BrushControl brushControl1;
        private System.Windows.Forms.Button button1;
        private ShiftControl shiftControl1;
        private SpecialInfoControl specialInfoControl1;
        private ShapesControl shapesControl1;
        private System.Windows.Forms.Label cursorLabel;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
    }
}

