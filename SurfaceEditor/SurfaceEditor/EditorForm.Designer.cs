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
            this.surfacePanel1 = new SurfaceEditor.SurfacePanel();
            this.SuspendLayout();
            // 
            // surfacePanel1
            // 
            this.surfacePanel1.Location = new System.Drawing.Point(13, 13);
            this.surfacePanel1.Name = "surfacePanel1";
            this.surfacePanel1.Size = new System.Drawing.Size(716, 480);
            this.surfacePanel1.TabIndex = 0;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(974, 511);
            this.Controls.Add(this.surfacePanel1);
            this.Name = "EditorForm";
            this.Text = "ASCIILib Surface Editor";
            this.ResumeLayout(false);

        }

        #endregion

        private SurfacePanel surfacePanel1;
    }
}

