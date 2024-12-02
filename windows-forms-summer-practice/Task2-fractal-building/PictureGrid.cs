using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Task2_FractalBuilding
{
    public class PictureGrid
    {
        public int Nrows {  get; private set; }
        public int Ncols { get; private set; }
        private int[] bounds { get; set; }
        public Point[,] Centers { get; private set; }
        public int[,,] Bounds { get; private set; }
        private Pen pen { get; set; }
        private System.Drawing.Drawing2D.DashStyle dashstyle { get; set; }


        public PictureGrid(int nrows, int ncols, int[] bounds, Color color, System.Drawing.Drawing2D.DashStyle dashstyle)
        {
            this.Nrows = nrows;
            this.Ncols = ncols;
            this.pen = new Pen(color);
            pen.DashStyle = dashstyle;
            this.dashstyle = dashstyle;
            this.bounds = bounds;

            Centers = new Point[nrows, ncols];
            Bounds = new int[nrows, ncols, 4];
        }

        public void Initialize()
        {
            // [0] - top bound
            // [1] - right bound
            // [2] - down bound
            // [3] - left bound

            int colsStep = (bounds[1] - bounds[3]) / Ncols;
            int rowsStep = (bounds[2] - bounds[0]) / Nrows;

            for (int i = 0; i < Nrows; ++i)
            {
                for (int j = 0; j < Ncols; ++j)
                {
                    Bounds[i, j, 0] = bounds[0] + rowsStep * i;
                    Bounds[i, j, 1] = bounds[3] + colsStep * (j + 1);
                    Bounds[i, j, 2] = bounds[0] + rowsStep * (i + 1);
                    Bounds[i, j, 3] = bounds[3] + colsStep * j;

                    Centers[i, j] = new Point(
                        (Bounds[i, j, 1] + Bounds[i, j, 3]) / 2,
                        (Bounds[i, j, 0] + Bounds[i, j, 2]) / 2);
                }
            }
        }

        public void Print(Graphics g)
        {
            int horizontalStep = (bounds[1] - bounds[3]) / Ncols;
            int verticalStep = (bounds[2] - bounds[0]) / Nrows;

            for (int i = 0; i <= Ncols; ++i)
            {
                g.DrawLine(pen,
                    new Point(bounds[3] + horizontalStep * i, bounds[0]),
                    new Point(bounds[3] + horizontalStep * i, bounds[2]));
            }

            for (int i = 0; i <= Nrows; ++i)
            {
                g.DrawLine(pen,
                    new Point(bounds[3], bounds[0] + verticalStep * i),
                    new Point(bounds[1], bounds[0] + verticalStep * i));
            }
        }
    }
}
