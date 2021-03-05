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
            OnPropertyChanged(nameof(Tiles));
        }

        public void Resize(int newWidth, int newHeight)
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
                    if (x < this.Width && y < this.Height)
                    {
                        newTiles[x, y] = this.Tiles[x, y];
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

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyName)
        {
            this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
    }
}
