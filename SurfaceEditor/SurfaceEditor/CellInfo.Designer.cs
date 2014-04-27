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
            this.SuspendLayout();
            // 
            // characterLabel
            // 
            this.characterLabel.AutoSize = true;
            this.characterLabel.Location = new System.Drawing.Point(45, 6);
            this.characterLabel.Name = "characterLabel";
            this.characterLabel.Size = new System.Drawing.Size(59, 13);
            this.characterLabel.TabIndex = 3;
            this.characterLabel.Text = "Character: ";
            // 
            // backColorLabel
            // 
            this.backColorLabel.AutoSize = true;
            this.backColorLabel.Location = new System.Drawing.Point(9, 58);
            this.backColorLabel.Name = "backColorLabel";
            this.backColorLabel.Size = new System.Drawing.Size(95, 13);
            this.backColorLabel.TabIndex = 4;
            this.backColorLabel.Text = "Background Color:";
            // 
            // charColorLabel
            // 
            this.charColorLabel.AutoSize = true;
            this.charColorLabel.Location = new System.Drawing.Point(21, 32);
            this.charColorLabel.Name = "charColorLabel";
            this.charColorLabel.Size = new System.Drawing.Size(83, 13);
            this.charColorLabel.TabIndex = 5;
            this.charColorLabel.Text = "Character Color:";
            // 
            // opacityLabel
            // 
            this.opacityLabel.AutoSize = true;
            this.opacityLabel.Location = new System.Drawing.Point(56, 84);
            this.opacityLabel.Name = "opacityLabel";
            this.opacityLabel.Size = new System.Drawing.Size(48, 13);
            this.opacityLabel.TabIndex = 6;
            this.opacityLabel.Text = "Opaque:";
            // 
            // infoLabel
            // 
            this.infoLabel.AutoSize = true;
            this.infoLabel.Location = new System.Drawing.Point(38, 110);
            this.infoLabel.Name = "infoLabel";
            this.infoLabel.Size = new System.Drawing.Size(66, 13);
            this.infoLabel.TabIndex = 7;
            this.infoLabel.Text = "Special Info:";
            // 
            // charTextBox
            // 
            this.charTextBox.Enabled = false;
            this.charTextBox.Location = new System.Drawing.Point(110, 3);
            this.charTextBox.Name = "charTextBox";
            this.charTextBox.Size = new System.Drawing.Size(87, 20);
            this.charTextBox.TabIndex = 8;
            // 
            // charColorTextBox
            // 
            this.charColorTextBox.Enabled = false;
            this.charColorTextBox.Location = new System.Drawing.Point(110, 29);
            this.charColorTextBox.Name = "charColorTextBox";
            this.charColorTextBox.Size = new System.Drawing.Size(87, 20);
            this.charColorTextBox.TabIndex = 9;
            // 
            // backColorTextBox
            // 
            this.backColorTextBox.Enabled = false;
            this.backColorTextBox.Location = new System.Drawing.Point(110, 55);
            this.backColorTextBox.Name = "backColorTextBox";
            this.backColorTextBox.Size = new System.Drawing.Size(87, 20);
            this.backColorTextBox.TabIndex = 10;
            // 
            // opacityTextBox
            // 
            this.opacityTextBox.Enabled = false;
            this.opacityTextBox.Location = new System.Drawing.Point(110, 81);
            this.opacityTextBox.Name = "opacityTextBox";
            this.opacityTextBox.Size = new System.Drawing.Size(87, 20);
            this.opacityTextBox.TabIndex = 11;
            // 
            // infoTextBox
            // 
            this.infoTextBox.Enabled = false;
            this.infoTextBox.Location = new System.Drawing.Point(110, 107);
            this.infoTextBox.Name = "infoTextBox";
            this.infoTextBox.Size = new System.Drawing.Size(87, 20);
            this.infoTextBox.TabIndex = 12;
            // 
            // CellInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightGray;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.infoTextBox);
            this.Controls.Add(this.opacityTextBox);
            this.Controls.Add(this.backColorTextBox);
            this.Controls.Add(this.charColorTextBox);
            this.Controls.Add(this.charTextBox);
            this.Controls.Add(this.infoLabel);
            this.Controls.Add(this.opacityLabel);
            this.Controls.Add(this.charColorLabel);
            this.Controls.Add(this.backColorLabel);
            this.Controls.Add(this.characterLabel);
            this.Name = "CellInfo";
            this.Size = new System.Drawing.Size(204, 134);
            this.ResumeLayout(false);
            this.PerformLayout();

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
    }
}
