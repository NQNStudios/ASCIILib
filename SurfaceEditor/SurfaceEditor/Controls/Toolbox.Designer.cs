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
            this.shiftButton = new System.Windows.Forms.RadioButton();
            this.resizeButton = new System.Windows.Forms.RadioButton();
            this.longTextButton = new System.Windows.Forms.RadioButton();
            this.textButton = new System.Windows.Forms.RadioButton();
            this.placeInfoButton = new System.Windows.Forms.RadioButton();
            this.toggleOpacityButton = new System.Windows.Forms.RadioButton();
            this.fillCellsButton = new System.Windows.Forms.RadioButton();
            this.eyeDropperButton = new System.Windows.Forms.RadioButton();
            this.shapesButton = new System.Windows.Forms.RadioButton();
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
            this.groupBox1.Controls.Add(this.shapesButton);
            this.groupBox1.Controls.Add(this.eyeDropperButton);
            this.groupBox1.Controls.Add(this.fillCellsButton);
            this.groupBox1.Controls.Add(this.shiftButton);
            this.groupBox1.Controls.Add(this.resizeButton);
            this.groupBox1.Controls.Add(this.longTextButton);
            this.groupBox1.Controls.Add(this.textButton);
            this.groupBox1.Controls.Add(this.placeInfoButton);
            this.groupBox1.Controls.Add(this.toggleOpacityButton);
            this.groupBox1.Controls.Add(this.viewCellsButton);
            this.groupBox1.Controls.Add(this.paintCellsButton);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(210, 236);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Mode";
            // 
            // shiftButton
            // 
            this.shiftButton.AutoSize = true;
            this.shiftButton.Location = new System.Drawing.Point(6, 204);
            this.shiftButton.Name = "shiftButton";
            this.shiftButton.Size = new System.Drawing.Size(86, 17);
            this.shiftButton.TabIndex = 8;
            this.shiftButton.TabStop = true;
            this.shiftButton.Text = "Shift Surface";
            this.shiftButton.UseVisualStyleBackColor = true;
            this.shiftButton.CheckedChanged += new System.EventHandler(this.shiftButton_CheckedChanged);
            // 
            // resizeButton
            // 
            this.resizeButton.AutoSize = true;
            this.resizeButton.Location = new System.Drawing.Point(6, 181);
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
            this.longTextButton.Location = new System.Drawing.Point(6, 158);
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
            this.textButton.Location = new System.Drawing.Point(6, 135);
            this.textButton.Name = "textButton";
            this.textButton.Size = new System.Drawing.Size(76, 17);
            this.textButton.TabIndex = 5;
            this.textButton.Text = "Place Text";
            this.textButton.UseVisualStyleBackColor = true;
            this.textButton.CheckedChanged += new System.EventHandler(this.textButton_CheckedChanged);
            // 
            // placeInfoButton
            // 
            this.placeInfoButton.AutoSize = true;
            this.placeInfoButton.Location = new System.Drawing.Point(6, 112);
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
            this.toggleOpacityButton.Location = new System.Drawing.Point(6, 88);
            this.toggleOpacityButton.Name = "toggleOpacityButton";
            this.toggleOpacityButton.Size = new System.Drawing.Size(97, 17);
            this.toggleOpacityButton.TabIndex = 2;
            this.toggleOpacityButton.Text = "Toggle Opacity";
            this.toggleOpacityButton.UseVisualStyleBackColor = true;
            this.toggleOpacityButton.CheckedChanged += new System.EventHandler(this.toggleOpacityButton_CheckedChanged);
            // 
            // fillCellsButton
            // 
            this.fillCellsButton.AutoSize = true;
            this.fillCellsButton.Location = new System.Drawing.Point(6, 65);
            this.fillCellsButton.Name = "fillCellsButton";
            this.fillCellsButton.Size = new System.Drawing.Size(62, 17);
            this.fillCellsButton.TabIndex = 9;
            this.fillCellsButton.TabStop = true;
            this.fillCellsButton.Text = "Fill Cells";
            this.fillCellsButton.UseVisualStyleBackColor = true;
            this.fillCellsButton.CheckedChanged += new System.EventHandler(this.fillCellsButton_CheckedChanged);
            // 
            // eyeDropperButton
            // 
            this.eyeDropperButton.AutoSize = true;
            this.eyeDropperButton.Location = new System.Drawing.Point(118, 19);
            this.eyeDropperButton.Name = "eyeDropperButton";
            this.eyeDropperButton.Size = new System.Drawing.Size(84, 17);
            this.eyeDropperButton.TabIndex = 10;
            this.eyeDropperButton.TabStop = true;
            this.eyeDropperButton.Text = "Eye Dropper";
            this.eyeDropperButton.UseVisualStyleBackColor = true;
            this.eyeDropperButton.CheckedChanged += new System.EventHandler(this.eyeDropperButton_CheckedChanged);
            // 
            // shapesButton
            // 
            this.shapesButton.AutoSize = true;
            this.shapesButton.Location = new System.Drawing.Point(118, 42);
            this.shapesButton.Name = "shapesButton";
            this.shapesButton.Size = new System.Drawing.Size(89, 17);
            this.shapesButton.TabIndex = 11;
            this.shapesButton.TabStop = true;
            this.shapesButton.Text = "Draw Shapes";
            this.shapesButton.UseVisualStyleBackColor = true;
            this.shapesButton.CheckedChanged += new System.EventHandler(this.shapesButton_CheckedChanged);
            // 
            // Toolbox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "Toolbox";
            this.Size = new System.Drawing.Size(326, 244);
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
        private System.Windows.Forms.RadioButton placeInfoButton;
        private System.Windows.Forms.RadioButton toggleOpacityButton;
        private System.Windows.Forms.RadioButton resizeButton;
        private System.Windows.Forms.RadioButton shiftButton;
        private System.Windows.Forms.RadioButton fillCellsButton;
        private System.Windows.Forms.RadioButton eyeDropperButton;
        private System.Windows.Forms.RadioButton shapesButton;
    }
}
