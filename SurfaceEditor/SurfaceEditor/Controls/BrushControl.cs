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
            charCheckBox.Checked = charTextBox.Text != "";
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
            get { return charTextBox.Text[0]; }
        }

        public Color BackgroundColor
        {
            get { return bgColorPicker.Color; }
        }

        public Color CharacterColor
        {
            get { return charColorPicker.Color; }
        }

        #endregion
    }
}
