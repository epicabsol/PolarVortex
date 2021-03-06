using System;
using System.Collections.Generic;
using System.Text;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class ResizeGridAction : MoveGridAction
    {
        public int WidthChange { get; }
        public int HeightChange { get; }
        public WorldTile[,] OldTiles { get; }

        public ResizeGridAction(int gridIndex, int xChange, int yChange, int widthChange, int heightChange, WorldTile[,] oldTiles) : base(gridIndex, xChange, yChange)
        {
            this.WidthChange = widthChange;
            this.HeightChange = heightChange;
            this.OldTiles = oldTiles;
        }

        public override void Execute(World world)
        {
            Grid grid = world.Grids[this.GridIndex];
            grid.Resize(grid.Width + this.WidthChange, grid.Height + this.HeightChange, -this.XChange, -this.YChange);
            base.Execute(world);
        }

        public override void Undo(World world)
        {
            base.Undo(world);
            Grid grid = world.Grids[this.GridIndex];
            grid.Resize(grid.Width - this.WidthChange, grid.Height - this.HeightChange, this.XChange, this.YChange);
            // If by undoing we have grown in either direction then we need to restore the tiles that were originally chopped off
            if (this.WidthChange < 0 || this.HeightChange < 0)
            {
                grid.UpdateTiles(this.OldTiles, 0, 0);
            }
        }

        public override string ToString()
        {
            return base.ToString() + " and increase the size by " + this.WidthChange + " × " + this.HeightChange;
        }
    }
}
