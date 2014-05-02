﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SurfaceEditor
{
    public partial class LongTextForm : Form
    {
        public LongTextForm()
        {
            InitializeComponent();
        }

        public string ChosenText
        {
            get { return textBox1.Text; }
        }

        public Color ChosenColor
        {
            get { return colorPickerControl1.Color; }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
