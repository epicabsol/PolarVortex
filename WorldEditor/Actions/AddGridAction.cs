using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class AddGridAction : Action
    {
        public int X { get; }
        public int Y { get; }
        public int Width { get; }
        public int Height { get; }
        public string PalettePath { get; }
        public int GridIndex { get; private set; }

        public AddGridAction(int x, int y, int width, int height, string palettePath, int gridIndex = -1)
        {
            this.X = x;
            this.Y = y;
            this.Width = width;
            this.Height = height;
            this.PalettePath = palettePath;
            this.GridIndex = gridIndex;
        }

        public override void Execute(World world)
        {
            this.GridIndex = world.Grids.Count;
            world.Grids.Add(new Grid(this.X, this.Y, this.PalettePath, this.Width, this.Height));
        }

        public override void Undo(World world)
        {
            world.Grids.RemoveAt(this.GridIndex);
        }

        public override string ToString()
        {
            return $"add a new grid at {this.X}, {this.Y} with size {this.Width} × {this.Height} and palette {this.PalettePath}";
        }
    }
}
