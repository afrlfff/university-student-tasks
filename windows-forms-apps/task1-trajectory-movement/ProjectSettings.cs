using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1_trajectory_movement
{
    public class ProjectSettings
    {
        public enum MoveDirection { FORWARD, BACK }

        public Color BackgroundColor { get; set; }
        public Pen TrajectoryPen { get; set; } 
        public Pen ObjectPen { get; set; }
        public Brush ObjectBrush { get; set; }
        public float[] ObjectPulsationValues { get; set; }
        public int ObjectPulsationsNumber {  get; set; }
        public MoveDirection ObjectMoveDirection { get; set; }
        public float ObjectMoveSpeed { get; set; }

        public ProjectSettings()
        {
            TrajectoryPen = new Pen(Color.Red);
            ObjectPen = new Pen(Color.Black);
            ObjectBrush = new System.Drawing.SolidBrush(Color.Transparent);
            BackgroundColor = Color.White;
            ObjectPulsationValues = new float[2] { 1.0f, 1.0f };
            ObjectPulsationsNumber = 5;
            ObjectMoveDirection = MoveDirection.FORWARD;
            ObjectMoveSpeed = 1.0f;
        }

        public ProjectSettings(ProjectSettings other)
        {
            TrajectoryPen = other.TrajectoryPen;
            ObjectPen = other.ObjectPen;
            ObjectBrush = other.ObjectBrush;
            BackgroundColor = other.BackgroundColor;
            ObjectPulsationValues = other.ObjectPulsationValues;
            ObjectPulsationsNumber = other.ObjectPulsationsNumber;
            ObjectMoveDirection = other.ObjectMoveDirection;
            ObjectMoveSpeed = other.ObjectMoveSpeed;
        }
    }
}
