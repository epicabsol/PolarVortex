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
                reader.Read(); // Now at the StartObject
                reader.Read(); // Now at the "palette" name
                reader.Read(); // Now at the palette value
                string palettePath = reader.ReadAsString(); // Now at the "width" name
                reader.Read();
                int width = reader.ReadAsInt32().Value; // Now at the "height" name
                reader.Read();
                int height = reader.ReadAsInt32().Value; // Now at the "tiles" name
                reader.Read(); // Now at array start
                reader.Read(); // Now at object start
                World result = new World(palettePath, width, height);
                int i = 0;
                while (reader.TokenType != JsonToken.EndArray)
                {
                    reader.Read(); // Now at "index" name
                    reader.Read(); // Now at index value
                    int paletteIndex = reader.ReadAsInt32().Value; // Now at "collides" name
                    reader.Read();
                    bool collides = reader.ReadAsBoolean().Value; // Now at end object
                    reader.Read(); // Now at object start or end array
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
