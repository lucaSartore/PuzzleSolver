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
        // say whether the user whant the program to go to the next state automatically when one calculation is finish
        private bool _goToNextStateAutomaticly = false;


        public MainAppFrom()
        {
            /// load all UI components
            InitializeComponent();
            // this is just to visualize tings, so i don't want to have inputs
            _stateVisualizer.Enabled = false;

            _state = State.OpenOrCreateFile;
            UpdateStatusView();

            // add the autematic resize of the image in the center
            Resize += ResizePreviewImage;
            ResizePreviewImage(null, null);

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
            // disable the menu
            runToolStripMenuItem.Enabled = false;
            runToolStripMenuItem.Text = "Run (already running)";


            Func<int> function_to_call;

            switch (_state)
            {
                case State.OpenOrCreateFile:
                    _ = MessageBox.Show("You need to open or create a file before doing calculations", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                case State.PieceSplitting:
                    function_to_call = _comunicator.SplitImage;
                    break;
                case State.CornerProcessing:
                    function_to_call = _comunicator.ProcessCorners;
                    break;
                case State.ConnectionProcessing:
                    function_to_call = _comunicator.CalculateConnections;
                    break;
                case State.CombinationFinding:
                    function_to_call = _comunicator.SolvePuzzle;
                    break;
                case State.Helping:
                    _ = MessageBox.Show("You have already done all possible calculations", "", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                default:
                    throw new ArgumentOutOfRangeException(paramName: "unknown state");
            }

            var task = Task.Run(() =>
            {
                // calculate the result in async thread
                var result = function_to_call();

                Invoke(() =>
                    // notify the main program when the calculation is finished
                    FinishCalculationTrigger(result));
            });

        }

        // trigger called when a calculation is finished
        private void FinishCalculationTrigger(int returnCode)
        {
            // update the status
            UpdateStatusView();

            // disable the menu
            runToolStripMenuItem.Enabled = true;
            runToolStripMenuItem.Text = "Run";

            // send messages based on return code
            if (returnCode < 0)
            {
                _ = MessageBox.Show("something went wrong in the calculation process. please check your input data", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // if user wants to go to the next state automatically
            if (_goToNextStateAutomaticly && _state != State.Helping)
            {
                // run the calculation one mor time
                RunToolStripMenuItem_Click(this, new EventArgs());
                return;
            }

            // give notification that the calculation has been completed without errors
            if (_state != State.CornerProcessing)
            {
                _ = MessageBox.Show("The calculation has finished with no error", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                _ = MessageBox.Show("I have found " + returnCode + " pieces in the images you gave me", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

        }


        /// resize the preview image when a resize event is receaved
        private void ResizePreviewImage(object sender, System.EventArgs e)
        {

            var p1 = _previewImageBox.Location;

            var p2 = ClientSize;

            _previewImageBox.Size = new Size(p2.Width - p1.X - 10, p2.Height - p1.Y - 10);
        }

        // enable the preview
        private void onToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_previewEnable == false)
            {
                // flag the preview as enable
                _previewEnable = true;
                // send message to enable it
                Comunicator.EnablePreview();
                // check the option
                onToolStripMenuItem.Checked = true;
                offToolStripMenuItem.Checked = false;
                _nextButton.Enabled = true;
            }
        }

        private void offToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_previewEnable == true)
            {
                // flag the preview as enable
                _previewEnable = false;
                // send message to enable it
                Comunicator.DisablePreview();
                // check the option
                onToolStripMenuItem.Checked = false;
                offToolStripMenuItem.Checked = true;
                _nextButton.Enabled = false;
            }
        }

        // disable go to next calculation step automatically
        private void offToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (_goToNextStateAutomaticly == true)
            {
                // flag the preview as enable
                _goToNextStateAutomaticly = false;
                // check the option
                onToolStripMenuItem1.Checked = false;
                offToolStripMenuItem1.Checked = true;
            }
        }
        // enable go to next calculation step automatically
        private void onToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (_goToNextStateAutomaticly == false)
            {
                // flag the preview as enable
                _goToNextStateAutomaticly = true;
                // check the option
                onToolStripMenuItem1.Checked = true;
                offToolStripMenuItem1.Checked = false;
            }
        }

        private void _nextButton_Click(object sender, EventArgs e)
        {
            if (_comunicator != null)
            {
                var image = _comunicator.GetNextPreviewImage();
                if (image != null)
                {
                    _previewImageBox.Image = image;
                }
            }
        }
    }
}
