using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Task1_trajectory_movement
{
    public partial class Form1 : Form
    {
        public static ProjectSettings mainProjectSettings;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Text = "Движение объекта";

            mainProjectSettings = new ProjectSettings(); // set default project settings
        }

        // caclulates "r" from input equation
        private double Calculate_r(double alpha, double phi)
        {
            return alpha * Math.Sin(3 * phi);
        }

        // caclculates radius of the object based on pulsation bounds and pulsation power
        private float Calculate_pulsatingRadius(float angle, float leftBound, float rightBound)
        {
            return (float)(leftBound + (rightBound - leftBound) * Math.Abs(Math.Sin(angle)));
        }

        private Point[] CalculateTrajectoryPoints(int N, int alpha)
        {
            double step = (1 * Math.PI / (N - 1));
            var variable = Pens.Black;

            // calculate degrees in radians and radiuses (polar coordinate system)
            double[] degrees = new double[N];
            double[] radiuses = new double[N];
            degrees[0] = 0.0;
            radiuses[0] = Calculate_r(alpha, 0);
            for (int i = 1; i < N; ++i)
            {
                degrees[i] = degrees[i - 1] + step;
                radiuses[i] = Calculate_r(alpha, degrees[i]);
            }

            // calculate result points (cartesian coordinate system)
            Point[] points = new Point[N];
            for (int i = 0; i < N; ++i)
            {
                points[i] = new Point(
                    Convert.ToInt32(radiuses[i] * Math.Cos(degrees[i])),
                    Convert.ToInt32(radiuses[i] * Math.Sin(degrees[i]))
                );
            }

            // convert points to be on the center of the picture
            int window_width = pictureBox1.Width;
            int window_height = pictureBox1.Height;
            for (int i = 0; i < N; ++i)
            {
                points[i] = new Point(
                    window_width / 2 + points[i].X,
                    window_height / 2 - points[i].Y);
            }

            return points;
        }

        // Paints graphics updating it every frame
        private void Update_Graphic(float ellipse_radius, Point[] points)
        {
            // inicialize graphic
            Graphics Графика = pictureBox1.CreateGraphics();
            Графика.Clear(mainProjectSettings.BackgroundColor);

            // coordinates of ellipse center
            int x0 = points[points.Length - 1].X;
            int y0 = points[points.Length - 1].Y;

            // add new piece (new frame) trajectory
            Графика.DrawLines(mainProjectSettings.TrajectoryPen, points);

            // print moving Ellipse
            // TODO: find out the idea of why the ellipse radius is divided by 2
            Графика.FillEllipse(mainProjectSettings.ObjectBrush, x0 - ellipse_radius / 2, y0 - ellipse_radius / 2, ellipse_radius, ellipse_radius);
            Графика.DrawEllipse(mainProjectSettings.ObjectPen, x0 - ellipse_radius / 2, y0 - ellipse_radius / 2, ellipse_radius, ellipse_radius);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            int N = 100; // number of points in trajectory (also number of frames in animation)
            int alpha = 200; // hyper parameter in equation
            int time_delay = 15; // delay between frames (in ms)

            Point[] points = CalculateTrajectoryPoints(N, alpha); // points for trajectory drawing
            if (mainProjectSettings.ObjectMoveDirection == ProjectSettings.MoveDirection.BACK)
            {
                points = points.Reverse().ToArray();
            } 

            float pulsatingRadius;
            float normal_raduis = alpha / 2;
            float pulsating_angle_step = (float)(Math.PI * mainProjectSettings.ObjectPulsationsNumber / (N - 1));

            // first frame (i == 0)
            Update_Graphic(0, new Point[] { points[0], points[1] });
            Thread.Sleep(time_delay);

            // start animation
            for (int i = 1; i < N; ++i)
            {
                pulsatingRadius = Calculate_pulsatingRadius(
                    pulsating_angle_step * i,
                    normal_raduis * mainProjectSettings.ObjectPulsationValues[0],
                    normal_raduis * mainProjectSettings.ObjectPulsationValues[1]);

                Update_Graphic(pulsatingRadius, points.Take(i + 1).ToArray());
                Thread.Sleep(Convert.ToInt32(time_delay / mainProjectSettings.ObjectMoveSpeed));
            }
        }

        private void параметрыToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SettingsForm settingsForm = new SettingsForm();
            settingsForm.ShowDialog();
        }

        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
