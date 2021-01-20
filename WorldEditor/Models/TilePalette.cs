using System;
using System.Collections.Generic;
using System.Text;

namespace WorldEditor.Models
{
    public class TilePalette
    {
        public string TexturePath { get; set; } = "";
        public int TileSize { get; set; } = 16;

        public TilePalette(string texturePath, int tileSize)
        {
            this.TexturePath = texturePath;
            this.TileSize = tileSize;
        }

        public static TilePalette LoadFromFile(string filename)
        {
            dynamic data = Newtonsoft.Json.JsonConvert.DeserializeObject(System.IO.File.ReadAllText(filename));
            string texturePath = data.texture;
            int tileSize = data["tile-size"];
            return new TilePalette(texturePath, tileSize);
        }
    }
}
