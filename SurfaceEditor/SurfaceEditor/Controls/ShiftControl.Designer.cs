namespace SurfaceEditor
{
    partial class ShiftControl
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
            this.leftButton = new System.Windows.Forms.Button();
            this.downButton = new System.Windows.Forms.Button();
            this.rightButton = new System.Windows.Forms.Button();
            this.upButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.leftButton);
            this.groupBox1.Controls.Add(this.downButton);
            this.groupBox1.Controls.Add(this.rightButton);
            this.groupBox1.Controls.Add(this.upButton);
            this.groupBox1.Location = new System.Drawing.Point(4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(85, 107);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Shift Surface";
            // 
            // leftButton
            // 
            this.leftButton.Location = new System.Drawing.Point(7, 48);
            this.leftButton.Name = "leftButton";
            this.leftButton.Size = new System.Drawing.Size(20, 23);
            this.leftButton.TabIndex = 3;
            this.leftButton.Text = "<";
            this.leftButton.UseVisualStyleBackColor = true;
            this.leftButton.Click += new System.EventHandler(this.leftButton_Click);
            // 
            // downButton
            // 
            this.downButton.Location = new System.Drawing.Point(30, 77);
            this.downButton.Name = "downButton";
            this.downButton.Size = new System.Drawing.Size(21, 23);
            this.downButton.TabIndex = 2;
            this.downButton.Text = "v";
            this.downButton.UseVisualStyleBackColor = true;
            this.downButton.Click += new System.EventHandler(this.downButton_Click);
            // 
            // rightButton
            // 
            this.rightButton.Location = new System.Drawing.Point(54, 48);
            this.rightButton.Name = "rightButton";
            this.rightButton.Size = new System.Drawing.Size(19, 23);
            this.rightButton.TabIndex = 1;
            this.rightButton.Text = ">";
            this.rightButton.UseVisualStyleBackColor = true;
            this.rightButton.Click += new System.EventHandler(this.rightButton_Click);
            // 
            // upButton
            // 
            this.upButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.upButton.Location = new System.Drawing.Point(30, 19);
            this.upButton.Name = "upButton";
            this.upButton.Size = new System.Drawing.Size(21, 23);
            this.upButton.TabIndex = 0;
            this.upButton.Text = "^";
            this.upButton.UseVisualStyleBackColor = true;
            this.upButton.Click += new System.EventHandler(this.upButton_Click);
            // 
            // ShiftControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ShiftControl";
            this.Size = new System.Drawing.Size(93, 116);
            this.Load += new System.EventHandler(this.ShiftControl_Load);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button leftButton;
        private System.Windows.Forms.Button downButton;
        private System.Windows.Forms.Button rightButton;
        private System.Windows.Forms.Button upButton;
    }
}
