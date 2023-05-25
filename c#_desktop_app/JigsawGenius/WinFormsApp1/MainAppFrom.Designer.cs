namespace JigsawGenius
{
    partial class MainAppFrom
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            if (disposing && (_comunicator != null))
            {
                _comunicator.Dispose();
            }

            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            _stateVisualizer = new CheckedListBox();
            menuStrip1 = new MenuStrip();
            toolStripMenuItem1 = new ToolStripMenuItem();
            saveToolStripMenuItem = new ToolStripMenuItem();
            openToolStripMenuItem = new ToolStripMenuItem();
            newToolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem2 = new ToolStripMenuItem();
            debugToolStripMenuItem = new ToolStripMenuItem();
            onToolStripMenuItem = new ToolStripMenuItem();
            offToolStripMenuItem = new ToolStripMenuItem();
            automaticContinueToolStripMenuItem = new ToolStripMenuItem();
            onToolStripMenuItem1 = new ToolStripMenuItem();
            offToolStripMenuItem1 = new ToolStripMenuItem();
            calculationsToolStripMenuItem = new ToolStripMenuItem();
            runToolStripMenuItem = new ToolStripMenuItem();
            deviceToolStripMenuItem = new ToolStripMenuItem();
            addToolStripMenuItem = new ToolStripMenuItem();
            aboutToolStripMenuItem = new ToolStripMenuItem();
            gitRepoToolStripMenuItem = new ToolStripMenuItem();
            helpToolStripMenuItem = new ToolStripMenuItem();
            _nextButtonPanel = new Panel();
            _nextButton = new Button();
            _previewImageBox = new PictureBox();
            menuStrip1.SuspendLayout();
            _nextButtonPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)_previewImageBox).BeginInit();
            SuspendLayout();
            // 
            // _stateVisualizer
            // 
            _stateVisualizer.FormattingEnabled = true;
            _stateVisualizer.Items.AddRange(new object[] { "Open or Create FIle", "Piece Splitter", "Corner Processer", "Connection Processer", "Combination Finder", "Assembly Helper" });
            _stateVisualizer.Location = new Point(12, 36);
            _stateVisualizer.Name = "_stateVisualizer";
            _stateVisualizer.Size = new Size(219, 172);
            _stateVisualizer.TabIndex = 0;
            // 
            // menuStrip1
            // 
            menuStrip1.ImageScalingSize = new Size(24, 24);
            menuStrip1.Items.AddRange(new ToolStripItem[] { toolStripMenuItem1, toolStripMenuItem2, calculationsToolStripMenuItem, deviceToolStripMenuItem, aboutToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(1060, 33);
            menuStrip1.TabIndex = 1;
            menuStrip1.Text = "menuStrip1";
            // 
            // toolStripMenuItem1
            // 
            toolStripMenuItem1.DropDownItems.AddRange(new ToolStripItem[] { saveToolStripMenuItem, openToolStripMenuItem, newToolStripMenuItem });
            toolStripMenuItem1.Name = "toolStripMenuItem1";
            toolStripMenuItem1.Size = new Size(54, 29);
            toolStripMenuItem1.Text = "File";
            // 
            // saveToolStripMenuItem
            // 
            saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            saveToolStripMenuItem.Size = new Size(176, 34);
            saveToolStripMenuItem.Text = "Save As";
            // 
            // openToolStripMenuItem
            // 
            openToolStripMenuItem.Name = "openToolStripMenuItem";
            openToolStripMenuItem.Size = new Size(176, 34);
            openToolStripMenuItem.Text = "Open";
            openToolStripMenuItem.Click += OpenToolStripMenuItem_Click;
            // 
            // newToolStripMenuItem
            // 
            newToolStripMenuItem.Name = "newToolStripMenuItem";
            newToolStripMenuItem.Size = new Size(176, 34);
            newToolStripMenuItem.Text = "New";
            newToolStripMenuItem.Click += NewToolStripMenuItem_Click;
            // 
            // toolStripMenuItem2
            // 
            toolStripMenuItem2.DropDownItems.AddRange(new ToolStripItem[] { debugToolStripMenuItem, automaticContinueToolStripMenuItem });
            toolStripMenuItem2.Name = "toolStripMenuItem2";
            toolStripMenuItem2.Size = new Size(92, 29);
            toolStripMenuItem2.Text = "Options";
            // 
            // debugToolStripMenuItem
            // 
            debugToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { onToolStripMenuItem, offToolStripMenuItem });
            debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            debugToolStripMenuItem.Size = new Size(332, 34);
            debugToolStripMenuItem.Text = "Preview";
            // 
            // onToolStripMenuItem
            // 
            onToolStripMenuItem.Name = "onToolStripMenuItem";
            onToolStripMenuItem.Size = new Size(140, 34);
            onToolStripMenuItem.Text = "On";
            onToolStripMenuItem.Click += onToolStripMenuItem_Click;
            // 
            // offToolStripMenuItem
            // 
            offToolStripMenuItem.Checked = true;
            offToolStripMenuItem.CheckState = CheckState.Checked;
            offToolStripMenuItem.Name = "offToolStripMenuItem";
            offToolStripMenuItem.Size = new Size(140, 34);
            offToolStripMenuItem.Text = "Off";
            offToolStripMenuItem.Click += offToolStripMenuItem_Click;
            // 
            // automaticContinueToolStripMenuItem
            // 
            automaticContinueToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { onToolStripMenuItem1, offToolStripMenuItem1 });
            automaticContinueToolStripMenuItem.Name = "automaticContinueToolStripMenuItem";
            automaticContinueToolStripMenuItem.Size = new Size(332, 34);
            automaticContinueToolStripMenuItem.Text = "After compleating a section";
            // 
            // onToolStripMenuItem1
            // 
            onToolStripMenuItem1.Name = "onToolStripMenuItem1";
            onToolStripMenuItem1.Size = new Size(261, 34);
            onToolStripMenuItem1.Text = "Go to next section";
            onToolStripMenuItem1.Click += onToolStripMenuItem1_Click;
            // 
            // offToolStripMenuItem1
            // 
            offToolStripMenuItem1.Checked = true;
            offToolStripMenuItem1.CheckState = CheckState.Checked;
            offToolStripMenuItem1.Name = "offToolStripMenuItem1";
            offToolStripMenuItem1.Size = new Size(261, 34);
            offToolStripMenuItem1.Text = "Wait for user input";
            offToolStripMenuItem1.Click += offToolStripMenuItem1_Click;
            // 
            // calculationsToolStripMenuItem
            // 
            calculationsToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { runToolStripMenuItem });
            calculationsToolStripMenuItem.Name = "calculationsToolStripMenuItem";
            calculationsToolStripMenuItem.Size = new Size(122, 29);
            calculationsToolStripMenuItem.Text = "Calculations";
            // 
            // runToolStripMenuItem
            // 
            runToolStripMenuItem.Name = "runToolStripMenuItem";
            runToolStripMenuItem.Size = new Size(270, 34);
            runToolStripMenuItem.Text = "Run";
            runToolStripMenuItem.Click += RunToolStripMenuItem_Click;
            // 
            // deviceToolStripMenuItem
            // 
            deviceToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { addToolStripMenuItem });
            deviceToolStripMenuItem.Name = "deviceToolStripMenuItem";
            deviceToolStripMenuItem.Size = new Size(80, 29);
            deviceToolStripMenuItem.Text = "Device";
            // 
            // addToolStripMenuItem
            // 
            addToolStripMenuItem.Name = "addToolStripMenuItem";
            addToolStripMenuItem.Size = new Size(148, 34);
            addToolStripMenuItem.Text = "Add";
            // 
            // aboutToolStripMenuItem
            // 
            aboutToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { gitRepoToolStripMenuItem, helpToolStripMenuItem });
            aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            aboutToolStripMenuItem.Size = new Size(78, 29);
            aboutToolStripMenuItem.Text = "About";
            // 
            // gitRepoToolStripMenuItem
            // 
            gitRepoToolStripMenuItem.Name = "gitRepoToolStripMenuItem";
            gitRepoToolStripMenuItem.Size = new Size(216, 34);
            gitRepoToolStripMenuItem.Text = "GitHub Repo";
            // 
            // helpToolStripMenuItem
            // 
            helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            helpToolStripMenuItem.Size = new Size(216, 34);
            helpToolStripMenuItem.Text = "Help";
            // 
            // _nextButtonPanel
            // 
            _nextButtonPanel.Anchor = AnchorStyles.Bottom | AnchorStyles.Left;
            _nextButtonPanel.Controls.Add(_nextButton);
            _nextButtonPanel.Location = new Point(0, 572);
            _nextButtonPanel.Name = "_nextButtonPanel";
            _nextButtonPanel.Size = new Size(231, 69);
            _nextButtonPanel.TabIndex = 2;
            // 
            // _nextButton
            // 
            _nextButton.Enabled = false;
            _nextButton.Location = new Point(12, 19);
            _nextButton.Name = "_nextButton";
            _nextButton.Size = new Size(219, 34);
            _nextButton.TabIndex = 0;
            _nextButton.Text = "Next Image";
            _nextButton.UseVisualStyleBackColor = true;
            _nextButton.Click += _nextButton_Click;
            // 
            // _previewImageBox
            // 
            _previewImageBox.BackColor = SystemColors.ActiveCaption;
            _previewImageBox.Location = new Point(237, 36);
            _previewImageBox.Name = "_previewImageBox";
            _previewImageBox.Size = new Size(807, 541);
            _previewImageBox.SizeMode = PictureBoxSizeMode.Zoom;
            _previewImageBox.TabIndex = 3;
            _previewImageBox.TabStop = false;
            // 
            // MainAppFrom
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1060, 640);
            Controls.Add(_previewImageBox);
            Controls.Add(_nextButtonPanel);
            Controls.Add(_stateVisualizer);
            Controls.Add(menuStrip1);
            MainMenuStrip = menuStrip1;
            Name = "MainAppFrom";
            Text = "JigsawGenius";
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            _nextButtonPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)_previewImageBox).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private CheckedListBox _stateVisualizer;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem toolStripMenuItem1;
        private ToolStripMenuItem toolStripMenuItem2;
        private ToolStripMenuItem saveToolStripMenuItem;
        private ToolStripMenuItem openToolStripMenuItem;
        private ToolStripMenuItem aboutToolStripMenuItem;
        private ToolStripMenuItem gitRepoToolStripMenuItem;
        private ToolStripMenuItem helpToolStripMenuItem;
        private ToolStripMenuItem newToolStripMenuItem;
        private ToolStripMenuItem debugToolStripMenuItem;
        private ToolStripMenuItem onToolStripMenuItem;
        private ToolStripMenuItem offToolStripMenuItem;
        private ToolStripMenuItem automaticContinueToolStripMenuItem;
        private ToolStripMenuItem onToolStripMenuItem1;
        private ToolStripMenuItem offToolStripMenuItem1;
        private ToolStripMenuItem deviceToolStripMenuItem;
        private ToolStripMenuItem addToolStripMenuItem;
        private ToolStripMenuItem calculationsToolStripMenuItem;
        private ToolStripMenuItem runToolStripMenuItem;
        private Panel _nextButtonPanel;
        private Button _nextButton;
        private PictureBox _previewImageBox;
    }
}
