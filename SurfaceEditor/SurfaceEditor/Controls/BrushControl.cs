using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SurfaceEditor
{
    public partial class BrushControl : UserControl
    {
        public BrushControl()
        {
            InitializeComponent();
        }

        private void BrushControl_Load(object sender, EventArgs e)
        {
            bgColorPicker.Color = Color.Black;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (charTextBox.Text.Length > 0)
            {
                textBox1.Text = "" + (int)charTextBox.Text[0];
                charCheckBox.Checked = true;
            }
            else
            {
                textBox1.Text = "";
                charCheckBox.Checked = false;
            }
        }

        #region Properties

        public bool PaintCharacter
        {
            get { return charCheckBox.Checked; }
        }

        public bool PaintBackgroundColor
        {
            get { return bgColorCheckBox.Checked; }
        }

        public bool PaintCharacterColor
        {
            get { return charColorCheckBox.Checked; }
        }

        public char Character
        {
            get 
            {
                if (charTextBox.Text.Length > 0)
                    return charTextBox.Text[0];
                else
                    return ' ';
            }
            set
            {
                charTextBox.Text = "" + value;
            }
        }

        public Color BackgroundColor
        {
            get { return bgColorPicker.Color; }
            set
            {
                bgColorPicker.Color = value;
            }
        }

        public Color CharacterColor
        {
            get { return charColorPicker.Color; }
            set
            {
                charColorPicker.Color = value;
            }
        }

        #endregion

        #region Special Modes

        public void BeginDropperMode()
        {
            charCheckBox.Enabled = true;
            charCheckBox.Checked = true;
            charTextBox.Enabled = false;
            bgColorPicker.Enabled = false;
            charColorPicker.Enabled = false;
        }

        public void EndDropperMode()
        {
            charCheckBox.Enabled = false;
            charCheckBox.Checked = charTextBox.Text.Length > 0;
            charTextBox.Enabled = true;
            bgColorPicker.Enabled = true;
            charColorPicker.Enabled = true;
        }

        public void BeginTextMode()
        {
            Visible = true;

            charCheckBox.Checked = false;
            charTextBox.Enabled = false;

            bgColorCheckBox.Checked = false;
            bgColorCheckBox.Enabled = false;

            bgColorPicker.Enabled = false;
            charColorCheckBox.Checked = true;
            charColorCheckBox.Enabled = false;
        }

        public void EndTextMode()
        {
            Visible = false;

            charCheckBox.Checked = false;
            charTextBox.Enabled = charTextBox.Text.Length > 0;

            bgColorCheckBox.Checked = true;
            bgColorCheckBox.Enabled = true;

            bgColorPicker.Enabled = true;
            charColorCheckBox.Checked = true;
            charColorCheckBox.Enabled = true;
        }

        #endregion

        private void useCharacterColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bgColorPicker.Color = charColorPicker.Color;
        }

        private void switchColorsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Color temp = bgColorPicker.Color;
            bgColorPicker.Color = charColorPicker.Color;
            charColorPicker.Color = temp;
        }

        private void useBackgroundColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            charColorPicker.Color = bgColorPicker.Color;
        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {
            if (textBox1.Text.Length > 0)
            {
                charTextBox.Text = "" + (char)int.Parse(textBox1.Text);
            }
        }
    }
}
