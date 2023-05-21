using Microsoft.VisualBasic.ApplicationServices;
using System.IO;
using System.Transactions;

namespace JigsawGenius
{

    /// all the pissuble state the image can have
    enum State { OpenOrCreateFile, InputData, PieceSplitting, CornerProcessing, ConnectionProcessing, CombinationFinding, Helping };

    public partial class MainAppFrom : Form
    {

        // instance that allows to comunicate with the c++ program
        private Comunicator? _comunicator = null;
        // the state of the program
        private State _state = State.OpenOrCreateFile;


        public MainAppFrom()
        {
            /// load all UI components
            InitializeComponent();
        }



        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // if i already have a file open i need to close it
            if (_comunicator != null)
            {
                _comunicator.Dispose();
            }

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "c:\\";
                openFileDialog.Filter = "status file (status.txt)|status.txt";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;
                openFileDialog.CheckFileExists = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFileDialog.FileName;
                    // remove the file and keep only the folder
                    filePath = filePath.Substring(0, filePath.Length - "status.txt".Length);


                    try
                    {
                        // open the folder
                        _comunicator = new Comunicator(filePath);
                    }
                    catch (FileLoadException)
                    {
                        MessageBox.Show("The file appear to be corrupted", "Sorry!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }

                    // update the state
                    // doto
                }
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // if i have a prokect open delete it
            if (_comunicator != null)
            {
                _comunicator.Dispose();
                _comunicator = null;
            }
            /// initialize the new comunicator
            CreateNewProjectFrom createNewProjectFrom = new CreateNewProjectFrom();
            createNewProjectFrom.InitializeProject(ref _state, ref _comunicator);

            // if the initialization has gone whell i tweak the threshold
            if(_comunicator != null)
            {
                // initialize the threshold
                ThresholdTweaking thresholdTweaking = new ThresholdTweaking(_comunicator);
                thresholdTweaking.ShowDialog();
            }
        }

        private void MainAppFrom_Load(object sender, EventArgs e)
        {

        }
    }
}