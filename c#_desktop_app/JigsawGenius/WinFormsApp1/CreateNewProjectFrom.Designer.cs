namespace JigsawGenius
{
    partial class CreateNewProjectFrom
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            _numberOfCoresSelector = new NumericUpDown();
            _resXSelector = new NumericUpDown();
            _resYSelector = new NumericUpDown();
            _numberOfCoresLable = new Label();
            label1 = new Label();
            label2 = new Label();
            textBoxInput = new TextBox();
            _inputPathLable = new Label();
            _outputPathLable = new Label();
            textBoxOutput = new TextBox();
            toolTip1 = new ToolTip(components);
            _infoText = new Label();
            buttonOpenInput = new Button();
            buttonOpenOutput = new Button();
            createButton = new Button();
            ((System.ComponentModel.ISupportInitialize)_numberOfCoresSelector).BeginInit();
            ((System.ComponentModel.ISupportInitialize)_resXSelector).BeginInit();
            ((System.ComponentModel.ISupportInitialize)_resYSelector).BeginInit();
            SuspendLayout();
            // 
            // _numberOfCoresSelector
            // 
            _numberOfCoresSelector.Location = new Point(314, 49);
            _numberOfCoresSelector.Maximum = new decimal(new int[] { 64, 0, 0, 0 });
            _numberOfCoresSelector.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            _numberOfCoresSelector.Name = "_numberOfCoresSelector";
            _numberOfCoresSelector.Size = new Size(104, 31);
            _numberOfCoresSelector.TabIndex = 0;
            _numberOfCoresSelector.Value = new decimal(new int[] { 1, 0, 0, 0 });
            _numberOfCoresSelector.ValueChanged += numericUpDown1_ValueChanged;
            // 
            // _resXSelector
            // 
            _resXSelector.Location = new Point(314, 117);
            _resXSelector.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            _resXSelector.Name = "_resXSelector";
            _resXSelector.Size = new Size(104, 31);
            _resXSelector.TabIndex = 1;
            _resXSelector.Value = new decimal(new int[] { 1, 0, 0, 0 });
            _resXSelector.ValueChanged += numericUpDown2_ValueChanged;
            // 
            // _resYSelector
            // 
            _resYSelector.Location = new Point(314, 154);
            _resYSelector.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            _resYSelector.Name = "_resYSelector";
            _resYSelector.Size = new Size(104, 31);
            _resYSelector.TabIndex = 2;
            _resYSelector.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // _numberOfCoresLable
            // 
            _numberOfCoresLable.AutoSize = true;
            _numberOfCoresLable.Location = new Point(40, 49);
            _numberOfCoresLable.Name = "_numberOfCoresLable";
            _numberOfCoresLable.Size = new Size(155, 25);
            _numberOfCoresLable.TabIndex = 3;
            _numberOfCoresLable.Text = "Number of cores: ";
            toolTip1.SetToolTip(_numberOfCoresLable, "the number of cores the program will use");
            _numberOfCoresLable.Click += label1_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(40, 117);
            label1.Name = "label1";
            label1.Size = new Size(217, 25);
            label1.TabIndex = 4;
            label1.Text = "Number of vertical pieces:";
            toolTip1.SetToolTip(label1, "how many pieces you there are in the vertical side of your puzzle");
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(40, 154);
            label2.Name = "label2";
            label2.Size = new Size(241, 25);
            label2.TabIndex = 5;
            label2.Text = "Number of horizontal pieces:";
            toolTip1.SetToolTip(label2, "how many pieces you there are in the horizontal side of your puzzle");
            // 
            // textBoxInput
            // 
            textBoxInput.Location = new Point(193, 261);
            textBoxInput.Name = "textBoxInput";
            textBoxInput.Size = new Size(441, 31);
            textBoxInput.TabIndex = 6;
            // 
            // _inputPathLable
            // 
            _inputPathLable.AutoSize = true;
            _inputPathLable.Location = new Point(40, 261);
            _inputPathLable.Name = "_inputPathLable";
            _inputPathLable.Size = new Size(110, 25);
            _inputPathLable.TabIndex = 7;
            _inputPathLable.Text = "Input folder:";
            toolTip1.SetToolTip(_inputPathLable, "a folder containing ONLY the scanned image (at 1200 ppi!) in a .png or jpeg format ");
            // 
            // _outputPathLable
            // 
            _outputPathLable.AutoSize = true;
            _outputPathLable.Location = new Point(40, 305);
            _outputPathLable.Name = "_outputPathLable";
            _outputPathLable.Size = new Size(125, 25);
            _outputPathLable.TabIndex = 8;
            _outputPathLable.Text = "Output folder:";
            toolTip1.SetToolTip(_outputPathLable, "the folder where you want to save the results. WARNING: if there are some files in the folder they will be deleted");
            _outputPathLable.Click += label3_Click;
            // 
            // textBoxOutput
            // 
            textBoxOutput.Location = new Point(193, 305);
            textBoxOutput.Name = "textBoxOutput";
            textBoxOutput.Size = new Size(441, 31);
            textBoxOutput.TabIndex = 9;
            // 
            // toolTip1
            // 
            toolTip1.ToolTipIcon = ToolTipIcon.Info;
            // 
            // _infoText
            // 
            _infoText.AutoSize = true;
            _infoText.ForeColor = Color.Red;
            _infoText.Location = new Point(457, 49);
            _infoText.Name = "_infoText";
            _infoText.Size = new Size(312, 150);
            _infoText.TabIndex = 10;
            _infoText.Text = "REMEMBER: in order for the program \r\nto work you need to scan the pieces\r\nat 1200 dpi on a black background.\r\n\r\nFor more informations\r\ngo to About > Help\r\n";
            // 
            // buttonOpenInput
            // 
            buttonOpenInput.Location = new Point(657, 261);
            buttonOpenInput.Name = "buttonOpenInput";
            buttonOpenInput.Size = new Size(112, 34);
            buttonOpenInput.TabIndex = 11;
            buttonOpenInput.Text = "Open";
            buttonOpenInput.UseVisualStyleBackColor = true;
            buttonOpenInput.Click += buttonOpenInput_Click;
            // 
            // buttonOpenOutput
            // 
            buttonOpenOutput.Location = new Point(657, 305);
            buttonOpenOutput.Name = "buttonOpenOutput";
            buttonOpenOutput.Size = new Size(112, 34);
            buttonOpenOutput.TabIndex = 12;
            buttonOpenOutput.Text = "Open";
            buttonOpenOutput.UseVisualStyleBackColor = true;
            buttonOpenOutput.Click += buttonOpenOutput_Click;
            // 
            // createButton
            // 
            createButton.Location = new Point(657, 364);
            createButton.Name = "createButton";
            createButton.Size = new Size(112, 34);
            createButton.TabIndex = 13;
            createButton.Text = "Create";
            createButton.UseVisualStyleBackColor = true;
            createButton.Click += createButton_Click;
            // 
            // CreateNewProjectFrom
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(820, 453);
            Controls.Add(createButton);
            Controls.Add(buttonOpenOutput);
            Controls.Add(buttonOpenInput);
            Controls.Add(_infoText);
            Controls.Add(textBoxOutput);
            Controls.Add(_outputPathLable);
            Controls.Add(_inputPathLable);
            Controls.Add(textBoxInput);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(_numberOfCoresLable);
            Controls.Add(_resYSelector);
            Controls.Add(_resXSelector);
            Controls.Add(_numberOfCoresSelector);
            Name = "CreateNewProjectFrom";
            Text = "Create new puzzle";
            Load += CreateNewProjectFrom_Load;
            ((System.ComponentModel.ISupportInitialize)_numberOfCoresSelector).EndInit();
            ((System.ComponentModel.ISupportInitialize)_resXSelector).EndInit();
            ((System.ComponentModel.ISupportInitialize)_resYSelector).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private NumericUpDown _numberOfCoresSelector;
        private NumericUpDown _resXSelector;
        private NumericUpDown _resYSelector;
        private Label _numberOfCoresLable;
        private Label label1;
        private Label label2;
        private TextBox textBoxInput;
        private Label _inputPathLable;
        private Label _outputPathLable;
        private TextBox textBoxOutput;
        private ToolTip toolTip1;
        private Label _infoText;
        private Button buttonOpenInput;
        private Button buttonOpenOutput;
        private Button createButton;
    }
}