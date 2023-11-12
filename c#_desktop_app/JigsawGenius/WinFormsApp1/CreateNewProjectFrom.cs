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
    partial class CreateNewProjectFrom : Form
    {
        private bool _confirmCreation = false;

        internal CreateNewProjectFrom()
        {
            InitializeComponent();
            this.TopMost = true;
            this.MaximumSize = new System.Drawing.Size(820, 470);
            this.MinimumSize = new System.Drawing.Size(820, 470);
        }

        internal void InitializeProject(ref State state, ref Comunicator? comunicator)
        {
            this.ShowDialog();

            // if the user has not confirm the creation no need to continue
            if (_confirmCreation == false)
            {
                return;
            }

            string input_path = this.textBoxInput.Text;
            string outptu_path = this.textBoxOutput.Text;

            uint number_of_cores = (uint)this._numberOfCoresSelector.Value;
            uint res_x = (uint)this._resXSelector.Value;
            uint res_y = (uint)this._resYSelector.Value;


            try
            {
                comunicator = new Comunicator(res_x, res_y, input_path, outptu_path, number_of_cores);
            }
            catch (FileLoadException)
            {
                MessageBox.Show("something went wrong while opening the files, check your pats and continue", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }catch (Exception)
            {
                // if the c++ library fails
                throw new UnknownDllLibrartError();
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {

        }

        private void CreateNewProjectFrom_Load(object sender, EventArgs e)
        {

        }

        private void _toolTipNumberOfCores_Popup(object sender, PopupEventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void _infoText_Click(object sender, EventArgs e)
        {

        }

        private void createButton_Click(object sender, EventArgs e)
        {
            // todo: verify that the data are correct
            _confirmCreation = true;
            this.Close();
        }

        private void buttonOpenInput_Click(object sender, EventArgs e)
        {
            /// need to open a new folder
            using (FolderBrowserDialog openFolderDialog = new FolderBrowserDialog())
            {

                openFolderDialog.InitialDirectory = "c:\\";

                if (openFolderDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFolderDialog.SelectedPath;
                    textBoxInput.Text = filePath;
                }
            }
        }

        private void buttonOpenOutput_Click(object sender, EventArgs e)
        {
            /// need to open a new folder
            using (FolderBrowserDialog openFolderDialog = new FolderBrowserDialog())
            {

                openFolderDialog.InitialDirectory = "c:\\";

                if (openFolderDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFolderDialog.SelectedPath;
                    textBoxOutput.Text = filePath;
                }
            }
        }
    }
}
