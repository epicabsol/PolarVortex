using System;
using System.Collections.Generic;
using System.Text;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class SetTileCollisionAction : Action
    {
        public int TileX { get; }
        public int TileY { get; }
        public bool OldCollision { get; }
        public bool NewCollision { get; }

        public SetTileCollisionAction(int tileX, int tileY, bool oldCollision, bool newCollision)
        {
            this.TileX = tileX;
            this.TileY = tileY;
            this.OldCollision = oldCollision;
            this.NewCollision = newCollision;
        }

        public override void Execute(World world)
        {
            world.Tiles[this.TileX, this.TileY].Collides = this.NewCollision;
        }

        public override void Undo(World world)
        {
            world.Tiles[this.TileX, this.TileY].Collides = this.OldCollision;
        }

        public override string ToString()
        {
            return $"set tile {this.TileX}, {this.TileY} collision to {this.NewCollision}";
        }
    }
}
