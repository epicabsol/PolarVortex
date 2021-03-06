using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace WorldEditor.Models
{
    public class Grid : INotifyPropertyChanged
    {
        private int _x = 0;
        public int X
        {
            get => _x;
            set
            {
                this._x = value;
                this.OnPropertyChanged(nameof(X));
            }
        }

        private int _y = 0;
        public int Y
        {
            get => _y;
            set
            {
                this._y = value;
                this.OnPropertyChanged(nameof(Y));
            }
        }

        private string _palettePath = "";
        public string PalettePath
        {
            get => this._palettePath;
            set
            {
                this._palettePath = value;
                this.OnPropertyChanged(nameof(PalettePath));
            }
        }

        private int _width;
        public int Width
        {
            get => this._width;
            private set
            {
                this._width = value;
                this.OnPropertyChanged(nameof(Width));
            }
        }

        private int _height;
        public int Height
        {
            get => this._height;
            private set
            {
                this._height = value;
                this.OnPropertyChanged(nameof(Height));
            }
        }

        private WorldTile[,] _tiles;
        public WorldTile[,] Tiles => _tiles;

        public Grid(int x, int y, string palettePath, int width, int height)
        {
            this._x = x;
            this._y = y;
            this._palettePath = palettePath;
            this._width = width;
            this._height = height;
            this._tiles = new WorldTile[this.Width, this.Height];

            for (int c = 0; c < this.Width; c++)
            {
                for (int r = 0; r < this.Height; r++)
                {
                    this.Tiles[c, r] = new WorldTile();
                    this.Tiles[c, r].PropertyChanged += Tile_PropertyChanged;
                }
            }
        }

        private void Tile_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            WorldTile tile = (WorldTile)sender;
            if (this.IsInBatchChanges)
            {
                this._haveTilesChanged = true;
            }
            else
            {
                OnPropertyChanged(nameof(Tiles));
            }
        }

        public void Resize(int newWidth, int newHeight, int xShift = 0, int yShift = 0)
        {
            for (int x = 0; x < this.Width; x++)
            {
                for (int y = 0; y < this.Height; y++)
                {
                    this.Tiles[x, y].PropertyChanged -= Tile_PropertyChanged;
                }
            }

            WorldTile[,] newTiles = new WorldTile[newWidth, newHeight];
            for (int x = 0; x < newWidth; x++)
            {
                for (int y = 0; y < newHeight; y++)
                {
                    if (x - xShift >= 0 && x - xShift < this.Width && y - yShift >= 0 && y - yShift < this.Height)
                    {
                        newTiles[x, y] = this.Tiles[x - xShift, y - yShift];
                    }
                    else
                    {
                        newTiles[x, y] = new WorldTile();
                    }
                    newTiles[x, y].PropertyChanged += Tile_PropertyChanged;
                }
            }
            this._width = newWidth;
            this._height = newHeight;
            this._tiles = newTiles;
            this.OnPropertyChanged(nameof(Width));
            this.OnPropertyChanged(nameof(Height));
            this.OnPropertyChanged(nameof(Tiles));
        }

        /// <summary>
        /// Generates a copy of each of the tiles in this Grid.
        /// </summary>
        /// <returns></returns>
        public WorldTile[,] CloneTiles()
        {
            WorldTile[,] result = new WorldTile[this.Width, this.Height];
            for (int x = 0; x < this.Width; x++)
            {
                for (int y = 0; y < this.Height; y++)
                {
                    result[x, y] = new WorldTile(this.Tiles[x, y].PaletteIndex, this.Tiles[x, y].Collides);
                }
            }
            return result;
        }

        /// <summary>
        /// Copies the given patch of tiles into this Grid at the given position.
        /// </summary>
        /// <param name="tiles"></param>
        public void UpdateTiles(WorldTile[,] tiles, int offsetX, int offsetY)
        {
            /*if (tiles.GetLength(0) != this.Tiles.GetLength(0) || tiles.GetLength(1) != this.Tiles.GetLength(1))
            {
                throw new ArgumentException("Mismatched array size. `tiles` should have the same dimensions as this Grid.");
            }*/

            int startX = offsetX > 0 ? offsetX : 0;
            int startY = offsetY > 0 ? offsetY : 0;
            int endX = this.Width < (tiles.GetLength(0) + offsetX) ? this.Width : (tiles.GetLength(0) + offsetX);
            int endY = this.Height < (tiles.GetLength(1) + offsetY) ? this.Height : (tiles.GetLength(1) + offsetY);

            this.BeginBatchChanges();
            for (int x = startX; x < endX; x++)
            {
                for (int y = startY; y < endY; y++)
                {
                    this.Tiles[x, y].PaletteIndex = tiles[x - offsetX, y - offsetY].PaletteIndex;
                    this.Tiles[x, y].Collides = tiles[x - offsetX, y - offsetY].Collides;
                }
            }
            this.EndBatchChanges();
        }

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyName)
        {
            this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private bool _haveTilesChanged = false;
        private int _batchChangeLevel = 0;
        public bool IsInBatchChanges => this._batchChangeLevel > 0;

        public void BeginBatchChanges()
        {
            if (this._batchChangeLevel == 0)
            {
                this._haveTilesChanged = false;
            }
            this._batchChangeLevel += 1;
        }

        public void EndBatchChanges()
        {
            this._batchChangeLevel -= 1;
            if (this._haveTilesChanged)
            {
                this.OnPropertyChanged(nameof(Tiles));
            }
        }
        #endregion
    }
}
