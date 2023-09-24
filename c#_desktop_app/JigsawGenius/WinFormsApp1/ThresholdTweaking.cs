using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace JigsawGenius
{

    public partial class ThresholdTweaking : Form
    {
        private Comunicator _comunicator;

        public ThresholdTweaking(Comunicator comunicator)
        {
            _comunicator = comunicator;
            InitializeComponent();
            // click the button once to create image preview
            _testButton_Click(null, null);

            // make it not resizable
            this.MinimumSize = new System.Drawing.Size(1158, 865);
            this.MaximumSize = new System.Drawing.Size(1158, 865);
        }

        private void _testButton_Click(object sender, EventArgs e)
        {
            _comunicator.SetSplitThreshold(thresholdTrackBar.Value);
            pictureBox1.Image = _comunicator.GetThresholdPreview();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
