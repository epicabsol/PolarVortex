using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Newtonsoft.Json;

namespace WorldEditor.Models
{
    public class World : INotifyPropertyChanged
    {
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

        public World(string palettePath, int width, int height)
        {
            this._palettePath = palettePath;
            this._width = width;
            this._height = height;
            this._tiles = new WorldTile[this.Width, this.Height];

            for (int x = 0; x < this.Width; x++)
            {
                for (int y = 0; y < this.Height; y++)
                {
                    this.Tiles[x, y] = new WorldTile();
                }
            }
        }

        public void Resize(int newWidth, int newHeight)
        {
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
                }
            }
            this._width = newWidth;
            this._height = newHeight;
            this._tiles = newTiles;
            this.OnPropertyChanged(nameof(Width));
            this.OnPropertyChanged(nameof(Height));
            this.OnPropertyChanged(nameof(Tiles));
        }

        public void WriteToFile(string filename)
        {
            // See https://www.newtonsoft.com/json/help/html/ReadingWritingJSON.htm
            throw new NotImplementedException();
        }

        public static World LoadFromFile(string filename)
        {
            using (System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Open))
            using (System.IO.TextReader textReader = new System.IO.StreamReader(stream))
            using (JsonReader reader = new JsonTextReader(textReader))
            {
                reader.Read();
                reader.Read();
                string palettePath = reader.ReadAsString();
                reader.Read();
                int width = reader.ReadAsInt32().Value;
                reader.Read();
                int height = reader.ReadAsInt32().Value;
                reader.Read();
                reader.Read();
                World result = new World(palettePath, width, height);
                int i = 0;
                reader.Read(); // Read the beginning of the object
                while (reader.TokenType != JsonToken.EndArray)
                {
                    reader.Read(); 
                    int paletteIndex = reader.ReadAsInt32().Value;
                    reader.Read();
                    bool collides = reader.ReadAsBoolean().Value; 
                    reader.Read(); 
                    reader.Read(); // Read the beginning of the next object or the end of the array
                    result.Tiles[i % width, i / width] = new WorldTile(paletteIndex, collides);

                    i++;
                }
                return result;
            }
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
