using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Task2_FractalBuilding
{
    public partial class ProjectSettingsForm : Form
    {
        private ProjectSettings tempProjectSettings;

        private void SettingsChanged(object sender, EventArgs e)
        {
            button_ПараметрыПрименить.Enabled = true;
        }

        private void UpdateFields(ProjectSettings settings)
        {
            numericUpDown_ЧислоШагов.Value = tempProjectSettings.StepsNumber + 1;
            
            numericUpDown_ШиринаИзображения.Value = Convert.ToInt32(tempProjectSettings.PictureSizeCoeffs[0] * 100);
            numericUpDown_ВысотаИзображения.Value = Convert.ToInt32(tempProjectSettings.PictureSizeCoeffs[1] * 100);
            
            numericUpDown_ДеревоВерхняяГраница.Value = Convert.ToInt32(tempProjectSettings.TreeBoundsCoeffs[0] * 100);
            numericUpDown_ДеревоНижняяГраница.Value = Convert.ToInt32(tempProjectSettings.TreeBoundsCoeffs[2] * 100);
            numericUpDown_ДеревоЛеваяГраница.Value = Convert.ToInt32(tempProjectSettings.TreeBoundsCoeffs[3] * 100);
            numericUpDown_ДеревоПраваяГраница.Value = Convert.ToInt32(tempProjectSettings.TreeBoundsCoeffs[1] * 100);

            numericUpDown_СеткаВерхняяГраница.Value = Convert.ToInt32(tempProjectSettings.GridBoundsCoeffs[0] * 100);
            numericUpDown_СеткаНижняяГраница.Value = Convert.ToInt32(tempProjectSettings.GridBoundsCoeffs[2] * 100);
            numericUpDown_СеткаЛеваяГраница.Value = Convert.ToInt32(tempProjectSettings.GridBoundsCoeffs[3] * 100);
            numericUpDown_СеткаПраваяГраница.Value = Convert.ToInt32(tempProjectSettings.GridBoundsCoeffs[1] * 100);

            numericUpDown_СеткаЧислоСтрок.Value = Convert.ToInt32(tempProjectSettings.GridRowsNumber);
            numericUpDown_СеткаЧислоСтолбцов.Value = Convert.ToInt32(tempProjectSettings.GridColsNumber);
        }

        private bool CorrectSettings(ProjectSettings settings)
        {
            if (settings.GridRowsNumber * settings.GridColsNumber < (settings.StepsNumber + 1))
            {
                return false;
            }
            return true;
        }


        public ProjectSettingsForm()
        {
            InitializeComponent();

            foreach (Control control in this.Controls)
            {
                if (control is FlowLayoutPanel)
                {
                    foreach (Control control2 in control.Controls)
                    {
                        if (control2 is TableLayoutPanel)
                        {
                            foreach (Control control3 in control2.Controls)
                            {
                                if (control3 is NumericUpDown)
                                {
                                    ((NumericUpDown)control3).ValueChanged += SettingsChanged;
                                }
                            }
                        }
                    }
                }
            }
        }

        private void ProjectSettingsForm_Load(object sender, EventArgs e)
        {
            tempProjectSettings = new ProjectSettings(MainForm.mainProjectSettings);
            UpdateFields(tempProjectSettings);

            button_ПараметрыПрименить.Enabled = false;
        }

        private void button_ПараметрыПрименить_Click(object sender, EventArgs e)
        {
            if (button_ПараметрыПрименить.Enabled)
            {
                if (CorrectSettings(tempProjectSettings))
                {
                    var result = MessageBox.Show("Сохранить измененмя?", "Подтвердите изменения", MessageBoxButtons.OKCancel);

                    if (result == DialogResult.OK)
                    {
                        MainForm.mainProjectSettings = new ProjectSettings(tempProjectSettings);
                        button_ПараметрыПрименить.Enabled = false;
                    }
                }
                else
                {
                    MessageBox.Show("Число ячеек в сетке не должно быть меньше числа шагов", "Ошибка", MessageBoxButtons.OK);
                }
            }
        }

        private void button_ПараметрыВыход_Click(object sender, EventArgs e)
        {
            if (button_ПараметрыПрименить.Enabled)
            {
                var result = MessageBox.Show("Сохранить измененbя?", "У вас есть несохраненные изменения", MessageBoxButtons.OKCancel);

                if (result == DialogResult.OK)
                {
                    MainForm.mainProjectSettings = new ProjectSettings(tempProjectSettings);
                }
            }
            this.Close();
        }

        private void numericUpDown_ЧислоШагов_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.StepsNumber = (int)numericUpDown_ЧислоШагов.Value - 1;
        }

        private void numericUpDown_ШиринаИзображения_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.PictureSizeCoeffs[0] = (float)(numericUpDown_ШиринаИзображения.Value / 100);
        }

        private void numericUpDown_ВысотаИзображения_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.PictureSizeCoeffs[1] = (float)(numericUpDown_ВысотаИзображения.Value / 100);
        }

        private void numericUpDown_ДеревоВерхняяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.TreeBoundsCoeffs[0] = (float)(numericUpDown_ДеревоВерхняяГраница.Value / 100);
        }

        private void numericUpDown_ДеревоНижняяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.TreeBoundsCoeffs[2] = (float)(numericUpDown_ДеревоНижняяГраница.Value / 100);
        }

        private void numericUpDown_ДеревоЛеваяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.TreeBoundsCoeffs[3] = (float)(numericUpDown_ДеревоЛеваяГраница.Value / 100);
        }

        private void numericUpDown_ДеревоПраваяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.TreeBoundsCoeffs[1] = (float)(numericUpDown_ДеревоПраваяГраница.Value / 100);
        }

        private void numericUpDown_СеткаВерхняяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.GridBoundsCoeffs[0] = (float)(numericUpDown_СеткаВерхняяГраница.Value / 100);
        }

        private void numericUpDown_СеткаНижняяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.GridBoundsCoeffs[2] = (float)(numericUpDown_СеткаНижняяГраница.Value / 100);
        }

        private void numericUpDown_СеткаЛеваяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.GridBoundsCoeffs[3] = (float)(numericUpDown_СеткаЛеваяГраница.Value / 100);
        }

        private void numericUpDown_СеткаПраваяГраница_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.GridBoundsCoeffs[1] = (float)(numericUpDown_СеткаПраваяГраница.Value / 100);
        }

        private void numericUpDown_СеткаЧислоСтрок_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.GridRowsNumber = (int)numericUpDown_СеткаЧислоСтрок.Value;
        }

        private void numericUpDown_СеткаЧислоСтолбцов_ValueChanged(object sender, EventArgs e)
        {
            tempProjectSettings.GridColsNumber = (int)numericUpDown_СеткаЧислоСтолбцов.Value;
        }
    }
}
