using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Task2_FractalBuilding
{
    public class ProjectSettings
    {
        public int StepsNumber { get; set; }
        public List<Color> StepsColors { get; set; }
        public float[] PictureSizeCoeffs { get; set; }
        public float[] TreeBoundsCoeffs { get; set; }
        public float[] GridBoundsCoeffs { get; set; }
        public int GridRowsNumber { get; set; }
        public int GridColsNumber { get; set; }

        public ProjectSettings()
        {
            StepsNumber = 4; // 0 step + 4 dividing steps (5 steps total)
            StepsColors = new List<Color>() {
                Color.Black, Color.Red, Color.Orange, Color.Green,
                Color.Blue, Color.Purple, Color.Yellow, Color.Red,
                Color.Orange, Color.Green };
            PictureSizeCoeffs = new float[2] { 1.0f, 1.0f };
            TreeBoundsCoeffs = new float[4] { 0.0f, 1.0f, 0.5f, 0.0f };
            GridBoundsCoeffs = new float[4] { 0.5f, 1.0f, 1.0f, 0.0f };

            GridRowsNumber = 1;
            GridColsNumber = 5;
        }

        public ProjectSettings(ProjectSettings other)
        {
            StepsNumber = other.StepsNumber;
            StepsColors = other.StepsColors;
            PictureSizeCoeffs = other.PictureSizeCoeffs;
            TreeBoundsCoeffs = other.TreeBoundsCoeffs;
            GridBoundsCoeffs = other.GridBoundsCoeffs;
            GridRowsNumber = other.GridRowsNumber;
            GridColsNumber = other.GridColsNumber;
        }
    }
}
