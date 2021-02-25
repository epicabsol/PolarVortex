using System;
using System.Collections.Generic;
using System.Text;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class SetTileAction : Action
    {
        public int TileX { get; }
        public int TileY { get; }
        public int OldPaletteIndex { get; }
        public int NewPaletteIndex { get; }

        public SetTileAction(int tileX, int tileY, int oldPaletteIndex, int newPaletteIndex)
        {
            this.TileX = tileX;
            this.TileY = tileY;
            this.OldPaletteIndex = oldPaletteIndex;
            this.NewPaletteIndex = newPaletteIndex;
        }

        public override void Execute(World world)
        {
            world.Tiles[this.TileX, this.TileY].PaletteIndex = this.NewPaletteIndex;
        }

        public override void Undo(World world)
        {
            world.Tiles[this.TileX, this.TileY].PaletteIndex = this.OldPaletteIndex;
        }

        public override string ToString()
        {
            return $"set tile {this.TileX}, {this.TileY} to {this.NewPaletteIndex}";
        }
    }
}
