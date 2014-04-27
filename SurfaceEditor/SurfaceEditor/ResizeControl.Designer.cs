namespace SurfaceEditor
{
    partial class ResizeControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.widthControl = new System.Windows.Forms.NumericUpDown();
            this.heightControl = new System.Windows.Forms.NumericUpDown();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.widthControl)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.heightControl)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.heightControl);
            this.groupBox1.Controls.Add(this.widthControl);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(133, 93);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Resize";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 54);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Height:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 27);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Width:";
            // 
            // widthControl
            // 
            this.widthControl.Location = new System.Drawing.Point(66, 25);
            this.widthControl.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.widthControl.Name = "widthControl";
            this.widthControl.Size = new System.Drawing.Size(37, 20);
            this.widthControl.TabIndex = 2;
            this.widthControl.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // heightControl
            // 
            this.heightControl.Location = new System.Drawing.Point(66, 52);
            this.heightControl.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.heightControl.Name = "heightControl";
            this.heightControl.Size = new System.Drawing.Size(37, 20);
            this.heightControl.TabIndex = 3;
            this.heightControl.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // ResizeControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ResizeControl";
            this.Size = new System.Drawing.Size(147, 104);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.widthControl)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.heightControl)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown heightControl;
        private System.Windows.Forms.NumericUpDown widthControl;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
    }
}
