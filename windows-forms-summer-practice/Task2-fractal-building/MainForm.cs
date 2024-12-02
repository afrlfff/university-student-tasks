using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Linq.Expressions;
using System.Net.Security;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Task2_FractalBuilding
{
    public partial class MainForm : Form
    {
        public static ProjectSettings mainProjectSettings;

        public MainForm()
        {
            InitializeComponent();

            mainProjectSettings = new ProjectSettings();
        }

        private void UpdateSettings()
        {
            // - 0.05f due to troubles with pictureBox sizing
            mainPictureBox.Size = new Size(
                (int)(flowLayoutPanel_main.Width * (mainProjectSettings.PictureSizeCoeffs[0] - 0.05f)),
                (int)(flowLayoutPanel_main.Height * (mainProjectSettings.PictureSizeCoeffs[1] - 0.05f)));
        }

        private void DrawDividingLines(Graphics g)
        {
            g.DrawRectangle(Pens.Black,
                (int)(mainPictureBox.Width * mainProjectSettings.TreeBoundsCoeffs[3]),
                (int)(mainPictureBox.Height * mainProjectSettings.TreeBoundsCoeffs[0]),
                (int)(mainPictureBox.Width * mainProjectSettings.TreeBoundsCoeffs[1] - mainPictureBox.Width * mainProjectSettings.TreeBoundsCoeffs[3]),
                (int)(mainPictureBox.Height * mainProjectSettings.TreeBoundsCoeffs[2] - mainPictureBox.Height * mainProjectSettings.TreeBoundsCoeffs[0]));

            g.DrawRectangle(Pens.Black,
                (int)(mainPictureBox.Width * mainProjectSettings.GridBoundsCoeffs[3]),
                (int)(mainPictureBox.Height * mainProjectSettings.GridBoundsCoeffs[0]),
                (int)(mainPictureBox.Width * mainProjectSettings.GridBoundsCoeffs[1] - mainPictureBox.Width * mainProjectSettings.GridBoundsCoeffs[3]),
                (int)(mainPictureBox.Height * mainProjectSettings.GridBoundsCoeffs[2] - mainPictureBox.Height * mainProjectSettings.GridBoundsCoeffs[0]));
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            UpdateSettings();
        }

        private void ToolStripMenuItem_Параметры_Click(object sender, EventArgs e)
        {
            ProjectSettingsForm settingsForm = new ProjectSettingsForm();
            settingsForm.Show();
        }

        private void ToolStripMenuItem_Выход_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ToolStripMenuItem_Запуск_Click(object sender, EventArgs e)
        {
            UpdateSettings();

            Image cachedImage = new Bitmap(mainPictureBox.Width, mainPictureBox.Height);
            Graphics g = Graphics.FromImage(cachedImage);
            g.Clear(Color.White);

            DrawDividingLines(g);

            FractalTree tree = new FractalTree(mainProjectSettings.StepsNumber);
            tree.Print(g, new int[4] {
                (int)(mainPictureBox.Height * mainProjectSettings.TreeBoundsCoeffs[0]),
                (int)(mainPictureBox.Width * mainProjectSettings.TreeBoundsCoeffs[1]),
                (int)(mainPictureBox.Height * mainProjectSettings.TreeBoundsCoeffs[2]),
                (int)(mainPictureBox.Width * mainProjectSettings.TreeBoundsCoeffs[3]) });

            PictureGrid grid = new PictureGrid(
                mainProjectSettings.GridRowsNumber,
                mainProjectSettings.GridColsNumber, new int[4] {
                (int)(mainPictureBox.Height * mainProjectSettings.GridBoundsCoeffs[0]),
                (int)(mainPictureBox.Width * mainProjectSettings.GridBoundsCoeffs[1]),
                (int)(mainPictureBox.Height * mainProjectSettings.GridBoundsCoeffs[2]),
                (int)(mainPictureBox.Width * mainProjectSettings.GridBoundsCoeffs[3]) }, 
                Color.Black, System.Drawing.Drawing2D.DashStyle.Solid);
            grid.Initialize();
            grid.Print(g);

            PrintFractalSteps(g, tree, grid);

            mainPictureBox.Image = cachedImage;
        }

        private void PrintFractalSteps(Graphics g, FractalTree tree, PictureGrid grid)
        {
            // pictureBounds
            // [0] - top bound
            // [1] - right bound
            // [2] - down bound
            // [3] - left bound

            List<FractalNode> nodesBFS = tree.BreadthFirstSearch();
            int row = 0, col = 0;

            for (int level = 0; level <= tree.Height; ++level)
            {
                List<FractalNode> currentNodes = new List<FractalNode>();
                for (int i = 0; i < tree.NumberOfNodesOnLevel(level); ++i)
                {
                    currentNodes.Add(nodesBFS.First());
                    nodesBFS.RemoveAt(0);
                }

                PrintFractalLevel(g, grid, row, col, currentNodes, level, tree.DiameterOnLevel(level));

                ++col;
                if (col == grid.Ncols)
                {
                    ++row;
                    col = 0;
                }
            }
        }

        private void PrintFractalLevel(Graphics g, PictureGrid grid, int row, int col, List<FractalNode> nodes, int level, int levelDiameter)
        {
            float scaleCoeff = Math.Min(
                (grid.Bounds[row, col, 1] - grid.Bounds[row, col, 3]) / (float)(levelDiameter),
                (grid.Bounds[row, col, 2] - grid.Bounds[row, col, 0]) / (float)(levelDiameter));
            
            float squareRadius = scaleCoeff / 2;

            if (squareRadius >= 1)
            {
                while (nodes.Count > 0)
                {
                    FractalNode currentNode = nodes.First();

                    PrintCross(g, mainProjectSettings.StepsColors.ElementAt(level),
                    new Point(
                        (int)(grid.Centers[row, col].X + currentNode.Center.X * scaleCoeff),
                        (int)(grid.Centers[row, col].Y - currentNode.Center.Y * scaleCoeff)),
                    (int)(squareRadius));

                    nodes.RemoveAt(0);
                }
            }
            else
            {
                while (nodes.Count > 0)
                {
                    FractalNode currentNode = nodes.First();

                    // minimum possible scaleCoeff is 2
                    Point newPoint = new Point(
                                (int)(grid.Centers[row, col].X + currentNode.Center.X * 2),
                                (int)(grid.Centers[row, col].Y - currentNode.Center.Y * 2));

                    // print if node places within a bounds
                    if (newPoint.X > grid.Bounds[row, col, 3] &&
                        newPoint.X < grid.Bounds[row, col, 1] &&
                        newPoint.Y > grid.Bounds[row, col, 0] &&
                        newPoint.Y < grid.Bounds[row, col, 2])
                    {
                        // minimum possible circleRadius is 1
                        PrintCross(g, mainProjectSettings.StepsColors.ElementAt(level), newPoint, 1);
                    }

                    nodes.RemoveAt(0);
                }
            }
        }

        private void PrintCross(Graphics g, Color color, Point center, float squareRadius)
        {   
            // center part
            g.FillRectangle(new SolidBrush(color),
                center.X - squareRadius, center.Y - squareRadius,
                squareRadius * 2, squareRadius * 2);

            // top part
            g.FillRectangle(new SolidBrush(color),
                center.X - squareRadius, center.Y - 3 * squareRadius,
                squareRadius * 2, squareRadius * 2);

            // right part
            g.FillRectangle(new SolidBrush(color),
                center.X + squareRadius, center.Y - squareRadius,
                squareRadius * 2, squareRadius * 2);

            // down part
            g.FillRectangle(new SolidBrush(color),
                center.X - squareRadius, center.Y + squareRadius,
                squareRadius * 2, squareRadius * 2);

            // left part
            g.FillRectangle(new SolidBrush(color),
                center.X - 3 * squareRadius, center.Y - squareRadius,
                squareRadius * 2, squareRadius * 2);
        }
    }
}
