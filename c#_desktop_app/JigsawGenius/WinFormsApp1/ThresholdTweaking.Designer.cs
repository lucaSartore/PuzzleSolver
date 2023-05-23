namespace JigsawGenius
{
    partial class ThresholdTweaking
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
            pictureBox1 = new PictureBox();
            thresholdTrackBar = new TrackBar();
            label1 = new Label();
            okButton = new Button();
            _testButton = new Button();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)thresholdTrackBar).BeginInit();
            SuspendLayout();
            // 
            // pictureBox1
            // 
            pictureBox1.Location = new Point(12, 9);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(657, 788);
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox1.TabIndex = 0;
            pictureBox1.TabStop = false;
            // 
            // thresholdTrackBar
            // 
            thresholdTrackBar.Location = new Point(675, 102);
            thresholdTrackBar.Maximum = 255;
            thresholdTrackBar.Name = "thresholdTrackBar";
            thresholdTrackBar.Size = new Size(449, 69);
            thresholdTrackBar.TabIndex = 1;
            thresholdTrackBar.Value = 100;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(675, 9);
            label1.Name = "label1";
            label1.Size = new Size(449, 75);
            label1.TabIndex = 2;
            label1.Text = "Please regulate the threshold so that all the backgroun\r\nbecame red, and only the puzzle pieces remain with the\r\noriginal color.";
            // 
            // okButton
            // 
            okButton.Location = new Point(1003, 763);
            okButton.Name = "okButton";
            okButton.Size = new Size(112, 34);
            okButton.TabIndex = 3;
            okButton.Text = "OK";
            okButton.UseVisualStyleBackColor = true;
            okButton.Click += okButton_Click;
            // 
            // _testButton
            // 
            _testButton.Location = new Point(1003, 158);
            _testButton.Name = "_testButton";
            _testButton.Size = new Size(112, 34);
            _testButton.TabIndex = 4;
            _testButton.Text = "Test";
            _testButton.UseVisualStyleBackColor = true;
            _testButton.Click += _testButton_Click;
            // 
            // ThresholdTweaking
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1136, 809);
            Controls.Add(_testButton);
            Controls.Add(okButton);
            Controls.Add(label1);
            Controls.Add(thresholdTrackBar);
            Controls.Add(pictureBox1);
            Name = "ThresholdTweaking";
            Text = "ThresholdTweaking";
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ((System.ComponentModel.ISupportInitialize)thresholdTrackBar).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private PictureBox pictureBox1;
        private TrackBar thresholdTrackBar;
        private Label label1;
        private Button okButton;
        private Button _testButton;
    }
}