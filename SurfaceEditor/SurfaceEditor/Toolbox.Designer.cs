namespace SurfaceEditor
{
    partial class Toolbox
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
            this.viewCellsButton = new System.Windows.Forms.RadioButton();
            this.paintCellsButton = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.resizeButton = new System.Windows.Forms.RadioButton();
            this.longTextButton = new System.Windows.Forms.RadioButton();
            this.textButton = new System.Windows.Forms.RadioButton();
            this.imagesButton = new System.Windows.Forms.RadioButton();
            this.placeInfoButton = new System.Windows.Forms.RadioButton();
            this.toggleOpacityButton = new System.Windows.Forms.RadioButton();
            this.shiftButton = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // viewCellsButton
            // 
            this.viewCellsButton.AutoSize = true;
            this.viewCellsButton.Checked = true;
            this.viewCellsButton.Location = new System.Drawing.Point(6, 19);
            this.viewCellsButton.Name = "viewCellsButton";
            this.viewCellsButton.Size = new System.Drawing.Size(85, 17);
            this.viewCellsButton.TabIndex = 0;
            this.viewCellsButton.TabStop = true;
            this.viewCellsButton.Text = "Inspect Cells";
            this.viewCellsButton.UseVisualStyleBackColor = true;
            this.viewCellsButton.CheckedChanged += new System.EventHandler(this.viewCellsButton_CheckedChanged);
            // 
            // paintCellsButton
            // 
            this.paintCellsButton.AutoSize = true;
            this.paintCellsButton.Location = new System.Drawing.Point(6, 42);
            this.paintCellsButton.Name = "paintCellsButton";
            this.paintCellsButton.Size = new System.Drawing.Size(74, 17);
            this.paintCellsButton.TabIndex = 1;
            this.paintCellsButton.Text = "Paint Cells";
            this.paintCellsButton.UseVisualStyleBackColor = true;
            this.paintCellsButton.CheckedChanged += new System.EventHandler(this.paintCellsButton_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.shiftButton);
            this.groupBox1.Controls.Add(this.resizeButton);
            this.groupBox1.Controls.Add(this.longTextButton);
            this.groupBox1.Controls.Add(this.textButton);
            this.groupBox1.Controls.Add(this.imagesButton);
            this.groupBox1.Controls.Add(this.placeInfoButton);
            this.groupBox1.Controls.Add(this.toggleOpacityButton);
            this.groupBox1.Controls.Add(this.viewCellsButton);
            this.groupBox1.Controls.Add(this.paintCellsButton);
            this.groupBox1.Location = new System.Drawing.Point(5, 5);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(147, 236);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Mode";
            // 
            // resizeButton
            // 
            this.resizeButton.AutoSize = true;
            this.resizeButton.Location = new System.Drawing.Point(6, 184);
            this.resizeButton.Name = "resizeButton";
            this.resizeButton.Size = new System.Drawing.Size(97, 17);
            this.resizeButton.TabIndex = 7;
            this.resizeButton.TabStop = true;
            this.resizeButton.Text = "Resize Surface";
            this.resizeButton.UseVisualStyleBackColor = true;
            this.resizeButton.CheckedChanged += new System.EventHandler(this.resizeButton_CheckedChanged);
            // 
            // longTextButton
            // 
            this.longTextButton.AutoSize = true;
            this.longTextButton.Location = new System.Drawing.Point(6, 160);
            this.longTextButton.Name = "longTextButton";
            this.longTextButton.Size = new System.Drawing.Size(103, 17);
            this.longTextButton.TabIndex = 6;
            this.longTextButton.Text = "Place Long Text";
            this.longTextButton.UseVisualStyleBackColor = true;
            this.longTextButton.CheckedChanged += new System.EventHandler(this.longTextButton_CheckedChanged);
            // 
            // textButton
            // 
            this.textButton.AutoSize = true;
            this.textButton.Location = new System.Drawing.Point(6, 137);
            this.textButton.Name = "textButton";
            this.textButton.Size = new System.Drawing.Size(76, 17);
            this.textButton.TabIndex = 5;
            this.textButton.Text = "Place Text";
            this.textButton.UseVisualStyleBackColor = true;
            this.textButton.CheckedChanged += new System.EventHandler(this.textButton_CheckedChanged);
            // 
            // imagesButton
            // 
            this.imagesButton.AutoSize = true;
            this.imagesButton.Location = new System.Drawing.Point(6, 114);
            this.imagesButton.Name = "imagesButton";
            this.imagesButton.Size = new System.Drawing.Size(80, 17);
            this.imagesButton.TabIndex = 4;
            this.imagesButton.Text = "Edit Images";
            this.imagesButton.UseVisualStyleBackColor = true;
            this.imagesButton.CheckedChanged += new System.EventHandler(this.imagesButton_CheckedChanged);
            // 
            // placeInfoButton
            // 
            this.placeInfoButton.AutoSize = true;
            this.placeInfoButton.Location = new System.Drawing.Point(6, 90);
            this.placeInfoButton.Name = "placeInfoButton";
            this.placeInfoButton.Size = new System.Drawing.Size(111, 17);
            this.placeInfoButton.TabIndex = 3;
            this.placeInfoButton.Text = "Place Special Info";
            this.placeInfoButton.UseVisualStyleBackColor = true;
            this.placeInfoButton.CheckedChanged += new System.EventHandler(this.placeInfoButton_CheckedChanged);
            // 
            // toggleOpacityButton
            // 
            this.toggleOpacityButton.AutoSize = true;
            this.toggleOpacityButton.Location = new System.Drawing.Point(6, 66);
            this.toggleOpacityButton.Name = "toggleOpacityButton";
            this.toggleOpacityButton.Size = new System.Drawing.Size(97, 17);
            this.toggleOpacityButton.TabIndex = 2;
            this.toggleOpacityButton.Text = "Toggle Opacity";
            this.toggleOpacityButton.UseVisualStyleBackColor = true;
            this.toggleOpacityButton.CheckedChanged += new System.EventHandler(this.toggleOpacityButton_CheckedChanged);
            // 
            // shiftButton
            // 
            this.shiftButton.AutoSize = true;
            this.shiftButton.Location = new System.Drawing.Point(6, 208);
            this.shiftButton.Name = "shiftButton";
            this.shiftButton.Size = new System.Drawing.Size(86, 17);
            this.shiftButton.TabIndex = 8;
            this.shiftButton.TabStop = true;
            this.shiftButton.Text = "Shift Surface";
            this.shiftButton.UseVisualStyleBackColor = true;
            // 
            // Toolbox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "Toolbox";
            this.Size = new System.Drawing.Size(159, 244);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RadioButton viewCellsButton;
        private System.Windows.Forms.RadioButton paintCellsButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton longTextButton;
        private System.Windows.Forms.RadioButton textButton;
        private System.Windows.Forms.RadioButton imagesButton;
        private System.Windows.Forms.RadioButton placeInfoButton;
        private System.Windows.Forms.RadioButton toggleOpacityButton;
        private System.Windows.Forms.RadioButton resizeButton;
        private System.Windows.Forms.RadioButton shiftButton;
    }
}
