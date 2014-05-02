namespace SurfaceEditor
{
    partial class CellInfo
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
            this.components = new System.ComponentModel.Container();
            this.characterLabel = new System.Windows.Forms.Label();
            this.backColorLabel = new System.Windows.Forms.Label();
            this.charColorLabel = new System.Windows.Forms.Label();
            this.opacityLabel = new System.Windows.Forms.Label();
            this.infoLabel = new System.Windows.Forms.Label();
            this.charTextBox = new System.Windows.Forms.TextBox();
            this.charColorTextBox = new System.Windows.Forms.TextBox();
            this.backColorTextBox = new System.Windows.Forms.TextBox();
            this.opacityTextBox = new System.Windows.Forms.TextBox();
            this.infoTextBox = new System.Windows.Forms.TextBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // characterLabel
            // 
            this.characterLabel.AutoSize = true;
            this.characterLabel.Location = new System.Drawing.Point(41, 23);
            this.characterLabel.Name = "characterLabel";
            this.characterLabel.Size = new System.Drawing.Size(59, 13);
            this.characterLabel.TabIndex = 3;
            this.characterLabel.Text = "Character: ";
            // 
            // backColorLabel
            // 
            this.backColorLabel.AutoSize = true;
            this.backColorLabel.Location = new System.Drawing.Point(5, 75);
            this.backColorLabel.Name = "backColorLabel";
            this.backColorLabel.Size = new System.Drawing.Size(95, 13);
            this.backColorLabel.TabIndex = 4;
            this.backColorLabel.Text = "Background Color:";
            // 
            // charColorLabel
            // 
            this.charColorLabel.AutoSize = true;
            this.charColorLabel.Location = new System.Drawing.Point(17, 49);
            this.charColorLabel.Name = "charColorLabel";
            this.charColorLabel.Size = new System.Drawing.Size(83, 13);
            this.charColorLabel.TabIndex = 5;
            this.charColorLabel.Text = "Character Color:";
            // 
            // opacityLabel
            // 
            this.opacityLabel.AutoSize = true;
            this.opacityLabel.Location = new System.Drawing.Point(52, 101);
            this.opacityLabel.Name = "opacityLabel";
            this.opacityLabel.Size = new System.Drawing.Size(48, 13);
            this.opacityLabel.TabIndex = 6;
            this.opacityLabel.Text = "Opaque:";
            // 
            // infoLabel
            // 
            this.infoLabel.AutoSize = true;
            this.infoLabel.Location = new System.Drawing.Point(34, 127);
            this.infoLabel.Name = "infoLabel";
            this.infoLabel.Size = new System.Drawing.Size(66, 13);
            this.infoLabel.TabIndex = 7;
            this.infoLabel.Text = "Special Info:";
            // 
            // charTextBox
            // 
            this.charTextBox.Enabled = false;
            this.charTextBox.Location = new System.Drawing.Point(106, 20);
            this.charTextBox.Name = "charTextBox";
            this.charTextBox.Size = new System.Drawing.Size(87, 20);
            this.charTextBox.TabIndex = 8;
            // 
            // charColorTextBox
            // 
            this.charColorTextBox.Enabled = false;
            this.charColorTextBox.Location = new System.Drawing.Point(106, 46);
            this.charColorTextBox.Name = "charColorTextBox";
            this.charColorTextBox.Size = new System.Drawing.Size(87, 20);
            this.charColorTextBox.TabIndex = 9;
            // 
            // backColorTextBox
            // 
            this.backColorTextBox.Enabled = false;
            this.backColorTextBox.Location = new System.Drawing.Point(106, 72);
            this.backColorTextBox.Name = "backColorTextBox";
            this.backColorTextBox.Size = new System.Drawing.Size(87, 20);
            this.backColorTextBox.TabIndex = 10;
            // 
            // opacityTextBox
            // 
            this.opacityTextBox.Enabled = false;
            this.opacityTextBox.Location = new System.Drawing.Point(106, 98);
            this.opacityTextBox.Name = "opacityTextBox";
            this.opacityTextBox.Size = new System.Drawing.Size(87, 20);
            this.opacityTextBox.TabIndex = 11;
            // 
            // infoTextBox
            // 
            this.infoTextBox.Enabled = false;
            this.infoTextBox.Location = new System.Drawing.Point(106, 124);
            this.infoTextBox.Name = "infoTextBox";
            this.infoTextBox.Size = new System.Drawing.Size(87, 20);
            this.infoTextBox.TabIndex = 12;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.opacityTextBox);
            this.groupBox1.Controls.Add(this.infoTextBox);
            this.groupBox1.Controls.Add(this.characterLabel);
            this.groupBox1.Controls.Add(this.backColorLabel);
            this.groupBox1.Controls.Add(this.backColorTextBox);
            this.groupBox1.Controls.Add(this.charColorLabel);
            this.groupBox1.Controls.Add(this.charColorTextBox);
            this.groupBox1.Controls.Add(this.opacityLabel);
            this.groupBox1.Controls.Add(this.charTextBox);
            this.groupBox1.Controls.Add(this.infoLabel);
            this.groupBox1.Location = new System.Drawing.Point(4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(199, 156);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Cell Info";
            // 
            // CellInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.Controls.Add(this.groupBox1);
            this.Name = "CellInfo";
            this.Size = new System.Drawing.Size(209, 167);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label characterLabel;
        private System.Windows.Forms.Label backColorLabel;
        private System.Windows.Forms.Label charColorLabel;
        private System.Windows.Forms.Label opacityLabel;
        private System.Windows.Forms.Label infoLabel;
        private System.Windows.Forms.TextBox charTextBox;
        private System.Windows.Forms.TextBox charColorTextBox;
        private System.Windows.Forms.TextBox backColorTextBox;
        private System.Windows.Forms.TextBox opacityTextBox;
        private System.Windows.Forms.TextBox infoTextBox;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}
