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

        public void ClearLabels()
        {
            listBox2.Items.Clear();
            listBox2.Items.Add("[Clear Info]");
        }

        public void LoadLabels(Surface surface)
        {
            for (int c = 0; c < surface.Width; ++c)
            {
                for (int r = 0; r < surface.Height; ++r)
                {
                    string label = surface.GetSpecialInfo(c, r);

                    if (label.Length > 0 && !listBox2.Items.Contains(label))
                    {
                        listBox2.Items.Add(label);
                    }
                }
            }
        }

        public void SelectLabel(string label)
        {
            listBox2.SelectedIndex = listBox2.Items.IndexOf(label);
        }
    }
}
