using System;
using System.Collections.Generic;
using System.Text;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class MoveGridAction : Action
    {
        public int GridIndex { get; }
        public int NewX { get; }
        public int NewY { get; }
        public int OldX { get; }
        public int OldY { get; }

        public MoveGridAction(int gridIndex, int newX, int newY, int oldX, int oldY)
        {
            this.GridIndex = gridIndex;
            this.NewX = newX;
            this.NewY = newY;
            this.OldX = oldX;
            this.OldY = oldY;
        }

        public override void Execute(World world)
        {
            world.Grids[this.GridIndex].X = this.NewX;
            world.Grids[this.GridIndex].Y = this.NewY;
        }

        public override void Undo(World world)
        {
            world.Grids[this.GridIndex].X = this.OldX;
            world.Grids[this.GridIndex].Y = this.OldY;
        }

        public override string ToString()
        {
            return $"move grid {this.GridIndex} to {this.NewX}, {this.NewY}";
        }
    }
}
