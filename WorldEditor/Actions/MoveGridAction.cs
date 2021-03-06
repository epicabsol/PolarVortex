using System;
using System.Collections.Generic;
using System.Text;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class MoveGridAction : Action
    {
        public int GridIndex { get; }
        public int XChange { get; }
        public int YChange { get; }

        public MoveGridAction(int gridIndex, int xChange, int yChange)
        {
            this.GridIndex = gridIndex;
            this.XChange = xChange;
            this.YChange = yChange;
        }

        public override void Execute(World world)
        {
            world.Grids[this.GridIndex].X += this.XChange;
            world.Grids[this.GridIndex].Y += this.YChange;
        }

        public override void Undo(World world)
        {
            world.Grids[this.GridIndex].X -= this.XChange;
            world.Grids[this.GridIndex].Y -= this.YChange;
        }

        public override string ToString()
        {
            return $"move grid {this.GridIndex} by {this.XChange}, {this.YChange}";
        }
    }
}
