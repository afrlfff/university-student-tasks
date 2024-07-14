namespace Task2_FractalBuilding
{
    partial class MainForm
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.ToolStripMenuItem_Файл = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem_Выход = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem_Редактирование = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem_Параметры = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem_Запуск = new System.Windows.Forms.ToolStripMenuItem();
            this.mainPictureBox = new System.Windows.Forms.PictureBox();
            this.flowLayoutPanel_main = new System.Windows.Forms.FlowLayoutPanel();
            this.menuStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mainPictureBox)).BeginInit();
            this.flowLayoutPanel_main.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItem_Файл,
            this.ToolStripMenuItem_Редактирование,
            this.ToolStripMenuItem_Запуск});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(1582, 30);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip1";
            // 
            // ToolStripMenuItem_Файл
            // 
            this.ToolStripMenuItem_Файл.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItem_Выход});
            this.ToolStripMenuItem_Файл.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ToolStripMenuItem_Файл.Name = "ToolStripMenuItem_Файл";
            this.ToolStripMenuItem_Файл.Size = new System.Drawing.Size(59, 26);
            this.ToolStripMenuItem_Файл.Text = "Файл";
            // 
            // ToolStripMenuItem_Выход
            // 
            this.ToolStripMenuItem_Выход.Name = "ToolStripMenuItem_Выход";
            this.ToolStripMenuItem_Выход.Size = new System.Drawing.Size(136, 26);
            this.ToolStripMenuItem_Выход.Text = "Выход";
            this.ToolStripMenuItem_Выход.Click += new System.EventHandler(this.ToolStripMenuItem_Выход_Click);
            // 
            // ToolStripMenuItem_Редактирование
            // 
            this.ToolStripMenuItem_Редактирование.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItem_Параметры});
            this.ToolStripMenuItem_Редактирование.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ToolStripMenuItem_Редактирование.Name = "ToolStripMenuItem_Редактирование";
            this.ToolStripMenuItem_Редактирование.Size = new System.Drawing.Size(137, 26);
            this.ToolStripMenuItem_Редактирование.Text = "Редактирование";
            // 
            // ToolStripMenuItem_Параметры
            // 
            this.ToolStripMenuItem_Параметры.Name = "ToolStripMenuItem_Параметры";
            this.ToolStripMenuItem_Параметры.Size = new System.Drawing.Size(173, 26);
            this.ToolStripMenuItem_Параметры.Text = "Параметры";
            this.ToolStripMenuItem_Параметры.Click += new System.EventHandler(this.ToolStripMenuItem_Параметры_Click);
            // 
            // ToolStripMenuItem_Запуск
            // 
            this.ToolStripMenuItem_Запуск.AutoSize = false;
            this.ToolStripMenuItem_Запуск.BackColor = System.Drawing.Color.Transparent;
            this.ToolStripMenuItem_Запуск.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ToolStripMenuItem_Запуск.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ToolStripMenuItem_Запуск.Image = ((System.Drawing.Image)(resources.GetObject("ToolStripMenuItem_Запуск.Image")));
            this.ToolStripMenuItem_Запуск.Name = "ToolStripMenuItem_Запуск";
            this.ToolStripMenuItem_Запуск.Padding = new System.Windows.Forms.Padding(0);
            this.ToolStripMenuItem_Запуск.Size = new System.Drawing.Size(80, 26);
            this.ToolStripMenuItem_Запуск.Text = "Запуск";
            this.ToolStripMenuItem_Запуск.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.ToolStripMenuItem_Запуск.Click += new System.EventHandler(this.ToolStripMenuItem_Запуск_Click);
            // 
            // mainPictureBox
            // 
            this.mainPictureBox.BackColor = System.Drawing.Color.White;
            this.mainPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.mainPictureBox.Location = new System.Drawing.Point(3, 3);
            this.mainPictureBox.Name = "mainPictureBox";
            this.mainPictureBox.Size = new System.Drawing.Size(1920, 700);
            this.mainPictureBox.TabIndex = 2;
            this.mainPictureBox.TabStop = false;
            // 
            // flowLayoutPanel_main
            // 
            this.flowLayoutPanel_main.AutoScroll = true;
            this.flowLayoutPanel_main.Controls.Add(this.mainPictureBox);
            this.flowLayoutPanel_main.Location = new System.Drawing.Point(12, 51);
            this.flowLayoutPanel_main.Name = "flowLayoutPanel_main";
            this.flowLayoutPanel_main.Size = new System.Drawing.Size(1495, 790);
            this.flowLayoutPanel_main.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1582, 853);
            this.Controls.Add(this.flowLayoutPanel_main);
            this.Controls.Add(this.menuStrip);
            this.ForeColor = System.Drawing.Color.Cornsilk;
            this.MainMenuStrip = this.menuStrip;
            this.MinimumSize = new System.Drawing.Size(1600, 900);
            this.Name = "MainForm";
            this.Text = "Построение фрактала \"Крест\"";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mainPictureBox)).EndInit();
            this.flowLayoutPanel_main.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem_Файл;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem_Выход;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem_Редактирование;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem_Параметры;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem_Запуск;
        private System.Windows.Forms.PictureBox mainPictureBox;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel_main;
    }
}

