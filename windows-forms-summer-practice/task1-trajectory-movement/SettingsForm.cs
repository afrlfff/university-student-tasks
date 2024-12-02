using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Button;

namespace Task1_trajectory_movement
{
    public partial class SettingsForm : Form
    {
        private ProjectSettings tempProjectSettings;


        // Checks if the values of settings are correct
        private bool UpdateSettings()
        {
            try
            {
                tempProjectSettings.ObjectPen.Width = Convert.ToInt32(textBox_объект_ТолщинаКонтура.Text);
                tempProjectSettings.ObjectPulsationValues[0] = Convert.ToInt32(textBox_объект_ПульсацияРадиусаОт.Text) / 100.0f;
                tempProjectSettings.ObjectPulsationValues[1] = Convert.ToInt32(textBox_объект_ПульсацияРадиусаДо.Text) / 100.0f;
                if (tempProjectSettings.ObjectPulsationValues[0] > tempProjectSettings.ObjectPulsationValues[1])
                {
                    return false;
                }
                tempProjectSettings.ObjectPulsationsNumber = Convert.ToInt32(textBox_объект_ЧислоПульсаций.Text);
                tempProjectSettings.ObjectMoveSpeed = Convert.ToInt32(textBox_объект_Скорость.Text) / 100.0f;

                tempProjectSettings.TrajectoryPen.Width = Convert.ToInt32(textBox_траектория_ТолщинаКонтура.Text);
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        private void SetLineTypeChecked(Pen pen, System.Windows.Forms.GroupBox lineTypeBox)
        {
            foreach (Control control in lineTypeBox.Controls)
            {
                if (control is System.Windows.Forms.RadioButton radioButton)
                {
                    if ((radioButton.Text == "Сплошной" && pen.DashStyle == System.Drawing.Drawing2D.DashStyle.Solid) ||
                        (radioButton.Text == "Пунктирный" && pen.DashStyle == System.Drawing.Drawing2D.DashStyle.Dash))
                    {
                        radioButton.Checked = true;
                    }
                    else
                    {
                        radioButton.Checked = false;
                    }
                }
            }
        }

        private void SetMoveDirectionChecked(ProjectSettings.MoveDirection direction, System.Windows.Forms.GroupBox lineTypeBox)
        {
            foreach (Control control in lineTypeBox.Controls)
            {
                if (control is System.Windows.Forms.RadioButton radioButton)
                {
                    if ((radioButton.Text == "Стандартное" && direction == ProjectSettings.MoveDirection.FORWARD) ||
                        (radioButton.Text == "Обратное" && direction == ProjectSettings.MoveDirection.BACK))
                    {
                        radioButton.Checked = true;
                    }
                    else
                    {
                        radioButton.Checked = false;
                    }
                }
            }
        }

        // ===============
        // ============================== ОСНОВНЫЕ ЭЛЕМЕНТЫ ============================== //
        // ===============

        public SettingsForm()
        {
            InitializeComponent();
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            // set default checkboxes and text values for object
            tempProjectSettings = new ProjectSettings(Form1.mainProjectSettings);
            textBox_объект_ТолщинаКонтура.Text = tempProjectSettings.ObjectPen.Width.ToString();
            SetLineTypeChecked(tempProjectSettings.ObjectPen, groupBox_объект_ТипКонтура);
            textBox_объект_ПульсацияРадиусаОт.Text = (tempProjectSettings.ObjectPulsationValues[0] * 100).ToString();
            textBox_объект_ПульсацияРадиусаДо.Text = (tempProjectSettings.ObjectPulsationValues[1] * 100).ToString();
            textBox_объект_ЧислоПульсаций.Text = tempProjectSettings.ObjectPulsationsNumber.ToString();
            SetMoveDirectionChecked(tempProjectSettings.ObjectMoveDirection, groupBox_объект_Направление);
            textBox_объект_ЧислоПульсаций.Text = tempProjectSettings.ObjectPulsationsNumber.ToString();
            textBox_объект_Скорость.Text = (tempProjectSettings.ObjectMoveSpeed * 100).ToString();

            // set default checkboxes and text values for trajectory
            textBox_траектория_ТолщинаКонтура.Text = tempProjectSettings.TrajectoryPen.Width.ToString();
            SetLineTypeChecked(tempProjectSettings.TrajectoryPen, groupBox_траектория_ТипЛинии);

            // default settings
            this.Text = "Параметры";
            flowLayoutPanel_объект.Visible = false;
            flowLayoutPanel_траектория.Visible = false;
            button_Применить.Enabled = false;
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            flowLayoutPanel_объект.Visible = true;
            flowLayoutPanel_траектория.Visible = false;
        }

        private void ToolStripMenuItem_траектория_Click(object sender, EventArgs e)
        {
            flowLayoutPanel_траектория.Visible = true;
            flowLayoutPanel_объект.Visible = false;
        }

        private void button_Применить_Click(object sender, EventArgs e)
        {
            if (UpdateSettings())
            {
                var result = MessageBox.Show("Сохранить изменения?", "Подтвердите изменения", MessageBoxButtons.OKCancel);

                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    Form1.mainProjectSettings = new ProjectSettings(tempProjectSettings);
                }
                button_Применить.Enabled = false;
            }
            else
            {
                MessageBox.Show("Есть некорректные значения", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button_Выход_Click(object sender, EventArgs e)
        {
            if (button_Применить.Enabled)
            {
                var result = MessageBox.Show("У вас есть несохраненные изменения.\nВы точно хотите выйти?",
                    "Подтвердите выход", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    this.Close();
                }
            }
            else
            {
                this.Close();
            }
        }



        // ===============
        // ============================== ОБЪЕКТ ============================== //
        // ===============

        private void button_объект_ВыбратьЦветЗаливки_Click(object sender, EventArgs e)
        {
            colorDialog1 = new ColorDialog();
            colorDialog1.Color = ((SolidBrush)tempProjectSettings.ObjectBrush).Color;
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                tempProjectSettings.ObjectBrush = new System.Drawing.SolidBrush(Color.FromArgb(
                    128, colorDialog1.Color.R, colorDialog1.Color.G, colorDialog1.Color.B));
                button_Применить.Enabled = true;
            }
        }

        private void button_объект_ВыбратьЦветКонтура_Click(object sender, EventArgs e)
        {
            colorDialog1 = new ColorDialog();
            colorDialog1.Color = tempProjectSettings.ObjectPen.Color;
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                tempProjectSettings.ObjectPen = new Pen(colorDialog1.Color, Form1.mainProjectSettings.ObjectPen.Width);
                button_Применить.Enabled = true;
            }
        }

        private void textBox_объект_ТолщинаКонтура_TextChanged(object sender, EventArgs e)
        {
            button_Применить.Enabled = true;
        }

        private void radioButton_объект_ТипЛинииСплошная_CheckedChanged(object sender, EventArgs e)
        {
            tempProjectSettings.ObjectPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;
            button_Применить.Enabled = true;
        }

        private void radioButton_объект_ТипЛинииПунктирная_CheckedChanged(object sender, EventArgs e)
        {
            tempProjectSettings.ObjectPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
            button_Применить.Enabled = true;
        }

        private void textBox_объект_ПульсацияРадиусаОт_TextChanged(object sender, EventArgs e)
        {
            button_Применить.Enabled = true;
        }

        private void textBox_объект_ПульсацияРадиусаДо_TextChanged(object sender, EventArgs e)
        {
            button_Применить.Enabled = true;
        }

        private void textBox_объект_ЧислоПульсаций_TextChanged(object sender, EventArgs e)
        {
            button_Применить.Enabled = true;
        }

        private void textBox_объект_Скорость_TextChanged(object sender, EventArgs e)
        {
            button_Применить.Enabled = true;
        }

        private void radioButton_объект_НаправлениеСтандартное_CheckedChanged(object sender, EventArgs e)
        {
            tempProjectSettings.ObjectMoveDirection = ProjectSettings.MoveDirection.FORWARD;
            button_Применить.Enabled = true;
        }

        private void radioButton_объект_НапрвлениеОбратное_CheckedChanged(object sender, EventArgs e)
        {
            tempProjectSettings.ObjectMoveDirection = ProjectSettings.MoveDirection.BACK;
            button_Применить.Enabled = true;
        }


        // ===============
        // ============================== ТРАЕКТОРИЯ ============================== //
        // =============== 

        private void button_траектория_ВыбратьЦвет_Click(object sender, EventArgs e)
        {
            colorDialog1 = new ColorDialog();
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                tempProjectSettings.TrajectoryPen = new Pen(colorDialog1.Color, Form1.mainProjectSettings.TrajectoryPen.Width);
                button_Применить.Enabled = true;
            }
        }

        private void textBox_траектория_ТолщинаКонтура_TextChanged(object sender, EventArgs e)
        {
            button_Применить.Enabled = true;
        }

        private void radioButton_траектория_ТипЛинииСплошной_CheckedChanged(object sender, EventArgs e)
        {
            tempProjectSettings.TrajectoryPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;
            button_Применить.Enabled = true;
        }

        private void radioButton_траектория_ТипЛинииПунктирный_CheckedChanged(object sender, EventArgs e)
        {
            tempProjectSettings.TrajectoryPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
            button_Применить.Enabled = true;
        }
    }
}
