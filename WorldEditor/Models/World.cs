using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Newtonsoft.Json;

namespace WorldEditor.Models
{
    public class World : INotifyPropertyChanged
    {
        public const string DialogFilter = "Polar Vortex Worlds (*.pvw)|*.pvw";

        public List<Grid> Grids { get; }

        public World(IEnumerable<Grid> grids)
        {
            this.Grids = new List<Grid>(grids);
        }

        public void WriteToFile(string filename)
        {
            // See https://www.newtonsoft.com/json/help/html/ReadingWritingJSON.htm
            using (System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Create))
            using (System.IO.StreamWriter textWriter = new System.IO.StreamWriter(stream))
            using (JsonWriter writer = new JsonTextWriter(textWriter))
            {
                writer.WriteStartObject(); // Start root

                writer.WritePropertyName("grids");
                writer.WriteStartArray(); // Start grids

                foreach (Grid grid in this.Grids)
                {
                    writer.WriteStartObject(); // Start grid

                    writer.WritePropertyName("x");
                    writer.WriteValue(grid.X);

                    writer.WritePropertyName("y");
                    writer.WriteValue(grid.Y);

                    writer.WritePropertyName("palette");
                    writer.WriteValue(grid.PalettePath);

                    writer.WritePropertyName("width");
                    writer.WriteValue(grid.Width);

                    writer.WritePropertyName("height");
                    writer.WriteValue(grid.Height);

                    writer.WritePropertyName("tiles");
                    writer.WriteStartArray(); // Start tiles

                    for (int y = 0; y < grid.Height; y++)
                    {
                        for (int x = 0; x < grid.Width; x++)
                        {
                            writer.WriteStartObject(); // Start tile

                            writer.WritePropertyName("index");
                            writer.WriteValue(grid.Tiles[x, y].PaletteIndex);

                            writer.WritePropertyName("collides");
                            writer.WriteValue(grid.Tiles[x, y].Collides);

                            writer.WriteEndObject(); // End tile
                        }
                    }

                    writer.WriteEndObject(); // End grid
                }
                writer.WriteEndArray(); // End grids

                writer.WriteEndArray(); // End tiles

                writer.WriteEndObject(); // End root
            }
        }

        public static World LoadFromFile(string filename)
        {
            using (System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Open))
            using (System.IO.TextReader textReader = new System.IO.StreamReader(stream))
            using (JsonReader reader = new JsonTextReader(textReader))
            {
                reader.Read(); // Start root object
                reader.Read(); // grids property
                reader.Read(); // Begin grids array
                reader.Read(); // Begin grid object or end of array
                List<Grid> grids = new List<Grid>();
                while (reader.TokenType != JsonToken.EndArray)
                {
                    reader.Read(); // x property
                    int x = reader.ReadAsInt32().Value;
                    reader.Read(); // y property
                    int y = reader.ReadAsInt32().Value;
                    reader.Read(); // palettePath property
                    string palettePath = reader.ReadAsString();
                    reader.Read();
                    int width = reader.ReadAsInt32().Value;
                    reader.Read();
                    int height = reader.ReadAsInt32().Value;
                    reader.Read(); // tiles property
                    reader.Read(); // start array
                    Grid grid = new Grid(x, y, palettePath, width, height);
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
                        grid.Tiles[i % width, i / width].PaletteIndex = paletteIndex;
                        grid.Tiles[i % width, i / width].Collides = collides;

                        i++;
                    }
                    reader.Read(); // End grid object
                    reader.Read(); // Begin next grid object or end of array

                    grids.Add(grid);
                }

                return new World(grids);
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
