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
    public partial class SpecialInfoControl : UserControl
    {
        public SpecialInfoControl()
        {
            InitializeComponent();
        }

        public string SelectedLabel
        {
            get { return (string)listBox2.Items[listBox2.SelectedIndex]; }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (textBox2.Text.Length > 0 && !listBox2.Items.Contains(textBox2.Text))
            {
                listBox2.Items.Add(textBox2.Text);
                listBox2.SelectedIndex = listBox2.Items.Count - 1;
            }

            textBox2.Text = "";
        }

        private void SpecialInfoControl_Load(object sender, EventArgs e)
        {
            listBox2.Items.Add("[Clear Info]");
        }
    }
}
