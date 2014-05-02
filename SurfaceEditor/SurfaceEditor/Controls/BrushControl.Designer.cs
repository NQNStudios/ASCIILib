namespace SurfaceEditor
{
    partial class BrushControl
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
            this.charTextBox = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.charColorCheckBox = new System.Windows.Forms.CheckBox();
            this.bgColorCheckBox = new System.Windows.Forms.CheckBox();
            this.charCheckBox = new System.Windows.Forms.CheckBox();
            this.bgColorPicker = new SurfaceEditor.ColorPickerControl();
            this.charColorPicker = new SurfaceEditor.ColorPickerControl();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // charTextBox
            // 
            this.charTextBox.Location = new System.Drawing.Point(6, 20);
            this.charTextBox.MaxLength = 1;
            this.charTextBox.Name = "charTextBox";
            this.charTextBox.Size = new System.Drawing.Size(20, 20);
            this.charTextBox.TabIndex = 0;
            this.charTextBox.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.charColorCheckBox);
            this.groupBox1.Controls.Add(this.bgColorCheckBox);
            this.groupBox1.Controls.Add(this.charCheckBox);
            this.groupBox1.Controls.Add(this.charTextBox);
            this.groupBox1.Controls.Add(this.bgColorPicker);
            this.groupBox1.Controls.Add(this.charColorPicker);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(154, 101);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Brush";
            // 
            // charColorCheckBox
            // 
            this.charColorCheckBox.AutoSize = true;
            this.charColorCheckBox.Checked = true;
            this.charColorCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.charColorCheckBox.Location = new System.Drawing.Point(34, 74);
            this.charColorCheckBox.Name = "charColorCheckBox";
            this.charColorCheckBox.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.charColorCheckBox.Size = new System.Drawing.Size(99, 17);
            this.charColorCheckBox.TabIndex = 6;
            this.charColorCheckBox.Text = "Character Color";
            this.charColorCheckBox.UseVisualStyleBackColor = true;
            // 
            // bgColorCheckBox
            // 
            this.bgColorCheckBox.AutoSize = true;
            this.bgColorCheckBox.Checked = true;
            this.bgColorCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.bgColorCheckBox.Location = new System.Drawing.Point(33, 48);
            this.bgColorCheckBox.Name = "bgColorCheckBox";
            this.bgColorCheckBox.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.bgColorCheckBox.Size = new System.Drawing.Size(111, 17);
            this.bgColorCheckBox.TabIndex = 5;
            this.bgColorCheckBox.Text = "Background Color";
            this.bgColorCheckBox.UseVisualStyleBackColor = true;
            // 
            // charCheckBox
            // 
            this.charCheckBox.AutoSize = true;
            this.charCheckBox.Enabled = false;
            this.charCheckBox.Location = new System.Drawing.Point(35, 23);
            this.charCheckBox.Name = "charCheckBox";
            this.charCheckBox.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.charCheckBox.Size = new System.Drawing.Size(72, 17);
            this.charCheckBox.TabIndex = 4;
            this.charCheckBox.Text = "Character";
            this.charCheckBox.UseVisualStyleBackColor = true;
            // 
            // bgColorPicker
            // 
            this.bgColorPicker.BackColor = System.Drawing.Color.White;
            this.bgColorPicker.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.bgColorPicker.Color = System.Drawing.Color.White;
            this.bgColorPicker.Location = new System.Drawing.Point(6, 46);
            this.bgColorPicker.Name = "bgColorPicker";
            this.bgColorPicker.Size = new System.Drawing.Size(20, 20);
            this.bgColorPicker.TabIndex = 1;
            // 
            // charColorPicker
            // 
            this.charColorPicker.BackColor = System.Drawing.Color.White;
            this.charColorPicker.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.charColorPicker.Color = System.Drawing.Color.White;
            this.charColorPicker.Location = new System.Drawing.Point(6, 73);
            this.charColorPicker.Name = "charColorPicker";
            this.charColorPicker.Size = new System.Drawing.Size(20, 20);
            this.charColorPicker.TabIndex = 2;
            // 
            // BrushControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "BrushControl";
            this.Size = new System.Drawing.Size(160, 112);
            this.Load += new System.EventHandler(this.BrushControl_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox charTextBox;
        private ColorPickerControl bgColorPicker;
        private ColorPickerControl charColorPicker;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox charColorCheckBox;
        private System.Windows.Forms.CheckBox bgColorCheckBox;
        private System.Windows.Forms.CheckBox charCheckBox;
    }
}
