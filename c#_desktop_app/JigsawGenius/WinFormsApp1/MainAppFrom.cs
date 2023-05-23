using Microsoft.VisualBasic.ApplicationServices;
using System.IO;
using System.Transactions;

namespace JigsawGenius
{

    /// all the possible states the image can have
    public enum State { OpenOrCreateFile, PieceSplitting, CornerProcessing, ConnectionProcessing, CombinationFinding, Helping };

    public partial class MainAppFrom : Form
    {

        // instance that allows to communicate with the c++ program
        private Comunicator? _comunicator = null;
        // the state of the program
        private State _state = State.OpenOrCreateFile;
        // say whether the program is currently running or not
        private bool _isRunning = false;
        // sey whether the preview is enable or not
        private bool _previewEnable = false;
        // say whether the calculations is running or nto
        private bool _isCalculationRunning = false;


        public MainAppFrom()
        {
            /// load all UI components
            InitializeComponent();
            // this is just to visualize tings, so i don't want to have inputs
            _stateVisualizer.Enabled = false;

            _state = State.OpenOrCreateFile;
            UpdateStatusView();
        }



        private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // if i already have a file open i need to close it
            if (_comunicator != null)
            {
                _comunicator.Dispose();
            }

            using (var openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "c:\\";
                openFileDialog.Filter = "status file (status.txt)|status.txt";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;
                openFileDialog.CheckFileExists = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    var filePath = openFileDialog.FileName;
                    // remove the file and keep only the folder
                    filePath = filePath.Substring(0, filePath.Length - "status.txt".Length);


                    try
                    {
                        // open the folder
                        _comunicator = new Comunicator(filePath);
                        UpdateStatusView();
                    }
                    catch (FileLoadException)
                    {
                        _ = MessageBox.Show("The file appear to be corrupted", "Sorry!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }


                }
            }
        }

        private void NewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // if i have a project open delete it
            if (_comunicator != null)
            {
                _comunicator.Dispose();
                _comunicator = null;
            }
            /// initialize the new communicator
            var createNewProjectFrom = new CreateNewProjectFrom();
            createNewProjectFrom.InitializeProject(ref _state, ref _comunicator);

            // if the initialization has gone well i tweak the threshold
            if (_comunicator != null)
            {
                // initialize the threshold
                var thresholdTweaking = new ThresholdTweaking(_comunicator);
                thresholdTweaking.ShowDialog();
                UpdateStatusView();
            }
        }

        // update the status view according to the status
        private void UpdateStatusView()
        {
            if (_comunicator != null)
            {
                _state = _comunicator.GetState();
            }
            else
            {
                _state = State.OpenOrCreateFile;
            }

            for (var i = 0; i < 6; i++)
            {
                _stateVisualizer.SetSelected(i, false);
            }

            switch (_state)
            {

                case State.OpenOrCreateFile:
                    _stateVisualizer.SetItemChecked(0, false);
                    _stateVisualizer.SetItemChecked(1, false);
                    _stateVisualizer.SetItemChecked(2, false);
                    _stateVisualizer.SetItemChecked(3, false);
                    _stateVisualizer.SetItemChecked(4, false);
                    _stateVisualizer.SetItemChecked(5, false);
                    _stateVisualizer.SetSelected(0, true);
                    break;
                case State.PieceSplitting:
                    _stateVisualizer.SetItemChecked(0, true);
                    _stateVisualizer.SetItemChecked(1, false);
                    _stateVisualizer.SetItemChecked(2, false);
                    _stateVisualizer.SetItemChecked(3, false);
                    _stateVisualizer.SetItemChecked(4, false);
                    _stateVisualizer.SetItemChecked(5, false);
                    _stateVisualizer.SetSelected(1, true);
                    break;
                case State.CornerProcessing:
                    _stateVisualizer.SetItemChecked(0, true);
                    _stateVisualizer.SetItemChecked(1, true);
                    _stateVisualizer.SetItemChecked(2, false);
                    _stateVisualizer.SetItemChecked(3, false);
                    _stateVisualizer.SetItemChecked(4, false);
                    _stateVisualizer.SetItemChecked(5, false);
                    _stateVisualizer.SetSelected(2, true);
                    break;
                case State.ConnectionProcessing:
                    _stateVisualizer.SetItemChecked(0, true);
                    _stateVisualizer.SetItemChecked(1, true);
                    _stateVisualizer.SetItemChecked(2, true);
                    _stateVisualizer.SetItemChecked(3, false);
                    _stateVisualizer.SetItemChecked(4, false);
                    _stateVisualizer.SetItemChecked(5, false);
                    _stateVisualizer.SetSelected(3, true);
                    break;
                case State.CombinationFinding:
                    _stateVisualizer.SetItemChecked(0, true);
                    _stateVisualizer.SetItemChecked(1, true);
                    _stateVisualizer.SetItemChecked(2, true);
                    _stateVisualizer.SetItemChecked(3, true);
                    _stateVisualizer.SetItemChecked(4, false);
                    _stateVisualizer.SetItemChecked(5, false);
                    _stateVisualizer.SetSelected(4, true);
                    break;
                case State.Helping:
                    _stateVisualizer.SetItemChecked(0, true);
                    _stateVisualizer.SetItemChecked(1, true);
                    _stateVisualizer.SetItemChecked(2, true);
                    _stateVisualizer.SetItemChecked(3, true);
                    _stateVisualizer.SetItemChecked(4, true);
                    _stateVisualizer.SetItemChecked(5, false);
                    _stateVisualizer.SetSelected(5, true);
                    break;
                default:
                    break;
            }
        }

        // run the next step of the calculation as an async task
        private void RunToolStripMenuItem_Click(object sender, EventArgs e)
        {


            Func<int> function_to_call;

            //public enum State { OpenOrCreateFile, PieceSplitting, CornerProcessing, ConnectionProcessing, CombinationFinding, Helping };
            switch (_state)
            {
                case State.OpenOrCreateFile:
                    _ = MessageBox.Show("You need to open or create a file before doing calculations", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;
                case State.PieceSplitting:
                    function_to_call = 
                    break;
                case State.CornerProcessing:
                    break;
                case State.ConnectionProcessing:
                    break;
                case State.CombinationFinding:
                    break;
                case State.Helping:
                    _ = MessageBox.Show("You have already done all possible calculations", "", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(paramName: "unknown state");
            }

            Task.Run(() =>
            {
                


            });
        }

        // trigger called when a calculation is finished
        private void FinishCalculationTrigger()
        {

        }
    }
}
