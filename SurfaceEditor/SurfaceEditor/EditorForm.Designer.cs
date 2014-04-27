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
            this.cellInfo1 = new SurfaceEditor.CellInfo();
            this.surfacePanel1 = new SurfaceEditor.SurfacePanel();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(974, 24);
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
            this.newSurfaceToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.newSurfaceToolStripMenuItem.Text = "&New Surface";
            // 
            // openSurfaceToolStripMenuItem
            // 
            this.openSurfaceToolStripMenuItem.Name = "openSurfaceToolStripMenuItem";
            this.openSurfaceToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.openSurfaceToolStripMenuItem.Text = "&Open Surface";
            this.openSurfaceToolStripMenuItem.Click += new System.EventHandler(this.openSurfaceToolStripMenuItem_Click);
            // 
            // saveSurfaceToolStripMenuItem
            // 
            this.saveSurfaceToolStripMenuItem.Name = "saveSurfaceToolStripMenuItem";
            this.saveSurfaceToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.saveSurfaceToolStripMenuItem.Text = "&Save Surface";
            // 
            // cellInfo1
            // 
            this.cellInfo1.BackColor = System.Drawing.Color.LightGray;
            this.cellInfo1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.cellInfo1.Location = new System.Drawing.Point(12, 391);
            this.cellInfo1.Name = "cellInfo1";
            this.cellInfo1.Size = new System.Drawing.Size(204, 134);
            this.cellInfo1.TabIndex = 2;
            // 
            // surfacePanel1
            // 
            this.surfacePanel1.Location = new System.Drawing.Point(12, 27);
            this.surfacePanel1.Name = "surfacePanel1";
            this.surfacePanel1.Size = new System.Drawing.Size(716, 358);
            this.surfacePanel1.Surface = null;
            this.surfacePanel1.TabIndex = 0;
            this.surfacePanel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.surfacePanel1_MouseMove);
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(974, 533);
            this.Controls.Add(this.cellInfo1);
            this.Controls.Add(this.surfacePanel1);
            this.Controls.Add(this.menuStrip1);
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
    }
}

