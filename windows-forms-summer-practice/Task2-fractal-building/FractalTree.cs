using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace Task2_FractalBuilding
{
    public class FractalNode
    {
        public Point Center { get; set; }
        public List<FractalNode> Children { get; set; }

        public FractalNode(Point Center)
        {
            this.Center = Center;
            Children = new List<FractalNode>();
        }

        public FractalNode(Point Center, List<FractalNode> Children)
        {
            this.Center = Center;
            this.Children = Children;
        }

        public FractalNode(FractalNode other)
        {
            this.Center = other.Center;
            this.Children = other.Children;
        }
    }

    // Representation of a fractal "Cross" where each cross contains 5 squares
    public class FractalTree
    {
        private readonly FractalNode root;
        public int Height { get; private set; }

        public FractalTree(int depth)
        {
            Height = depth;
            root = CreateFractalNode(new FractalNode(new Point(0, 0)), 0);
        }

        // Level of the tree starts from 0
        public int NumberOfNodesOnLevel(int level)
        {
            return (int)Math.Pow(5, level);
        }

        // Returns diameter of fractal (number of squares in diameter).
        // (level starts from 0).
        public int DiameterOnLevel(int level)
        {
            int diameter = 3;
            for (int i = 0; i < level; ++i)
            {
                diameter *= 3;
            }
            return diameter;
        }

        private FractalNode CreateFractalNode(FractalNode node, int currentLevel)
        {
            if (currentLevel == Height)
            {
                return node;
            }

            int childrenOffset = (int)Math.Pow(3, currentLevel + 1);

            node.Children.Add(
                CreateFractalNode(
                    new FractalNode(new Point(node.Center.X, node.Center.Y)),  currentLevel + 1
                ));
            node.Children.Add(
                CreateFractalNode(
                    new FractalNode(new Point(node.Center.X, node.Center.Y + childrenOffset)), currentLevel + 1
                ));
            node.Children.Add(
                CreateFractalNode(
                    new FractalNode(new Point(node.Center.X + childrenOffset, node.Center.Y)), currentLevel + 1
                ));
            node.Children.Add(
                CreateFractalNode(
                    new FractalNode(new Point(node.Center.X, node.Center.Y - childrenOffset)), currentLevel + 1
                ));
            node.Children.Add(
                CreateFractalNode(
                    new FractalNode(new Point(node.Center.X - childrenOffset, node.Center.Y)), currentLevel + 1
                ));

            return node;
        }

        public void Print(Graphics g, int[] pictuteBounds)
        {
            // pictureBounds
            // [0] - top bound
            // [1] - right bound
            // [2] - down bound
            // [3] - left bound

            // assume we print tree in a table where
            // every node takes his own cell
            float cellHeight = (pictuteBounds[2] - pictuteBounds[0]) / (float)(Height + 1);
            float cellWidth;

            // offset for drawing circles within a cells
            float verticalOffset = 0.05f;

            float minCircleRadius = cellHeight * 0.05f;

            List<Point> prevLevelNodesCenters = new List<Point>();
            float prevLevelCicrcleRadius = 0;

            for (int i = 0; i < (Height + 1); ++i)
            {
                int nodesNumber = NumberOfNodesOnLevel(i);
                cellWidth = (pictuteBounds[1] - pictuteBounds[3]) / (float)(nodesNumber);
                float circleRadius = (
                    Math.Min(cellWidth, cellHeight) - 
                    2 * verticalOffset * Math.Min(cellWidth, cellHeight) 
                    ) / 2;
                circleRadius = Math.Max(circleRadius, minCircleRadius);

                // Drawing
                for (int j = 0; j < nodesNumber; ++j)
                {
                    Point circleCenter = new Point(
                        pictuteBounds[3] + (int)(cellWidth * j + cellWidth / 2),
                        pictuteBounds[0] + (int)(cellHeight * i + cellHeight / 2));

                    // also if i > 0
                    if (prevLevelNodesCenters.Count > 0)
                    {
                        g.DrawLine(Pens.Black,
                            new Point(circleCenter.X, (int)(circleCenter.Y - circleRadius)), 
                            new Point(prevLevelNodesCenters.First().X, (int)(prevLevelNodesCenters.First().Y + prevLevelCicrcleRadius))
                            );
                        if ((j + 1) % 5 == 0)
                        {
                            prevLevelNodesCenters.RemoveAt(0);
                        }
                    }

                    g.FillEllipse(new SolidBrush(MainForm.mainProjectSettings.StepsColors.ElementAt(i)),
                        circleCenter.X - circleRadius,
                        circleCenter.Y - circleRadius,
                        circleRadius * 2, circleRadius * 2);

                    prevLevelNodesCenters.Add(circleCenter);
                }

                prevLevelCicrcleRadius = circleRadius;
            }
        }

        public List<FractalNode> BreadthFirstSearch()
        {
            List<FractalNode> visited = new List<FractalNode>();
            Queue<FractalNode> queue = new Queue<FractalNode>();

            if (root != null)
            {
                queue.Enqueue(root);
            }

            while (queue.Count > 0)
            {
                FractalNode current = queue.Dequeue();
                visited.Add(current);

                foreach (FractalNode child in current.Children)
                {
                    queue.Enqueue(child);
                }
            }

            return visited;
        }
    }
}
