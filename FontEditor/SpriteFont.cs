using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace FontEditor
{
    public class SpriteFontGlyph : INotifyPropertyChanged
    {
        private char _character;
        [JsonPropertyName("character")]
        public char Character
        { 
            get => _character;
            set
            {
                _character = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Character)));
            }
        }
        private float _x;
        [JsonPropertyName("x")]
        public float X
        {
            get => _x;
            set
            {
                _x = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(X)));
            }
        }
        private float _y;
        [JsonPropertyName("y")]
        public float Y
        {
            get => _y;
            set
            {
                _y = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Y)));
            }
        }
        private float _width;
        [JsonPropertyName("width")]
        public float Width
        {
            get => _width;
            set
            {
                _width = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Width)));
            }
        }
        private float _height;
        [JsonPropertyName("height")]
        public float Height
        {
            get => _height;
            set
            {
                _height = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Height)));
            }
        }
        private float _top;
        [JsonPropertyName("top")]
        public float Top
        {
            get => _top;
            set
            {
                _top = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Top)));
            }
        }
        private float _left;
        [JsonPropertyName("left")]
        public float Left
        {
            get => _left;
            set
            {
                _left = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Left)));
            }
        }
        private float _advance;
        [JsonPropertyName("advance")]
        public float Advance
        {
            get => _advance;
            set
            {
                _advance = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Advance)));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public SpriteFontGlyph(char c)
        {
            this.Character = c;
        }

        public SpriteFontGlyph()
        {

        }
    }

    public class SpriteFont
    {
        [JsonPropertyName("line-height")]
        public float LineHeight { get; set; } = 16;
        [JsonPropertyName("leading")]
        public float Leading { get; set; } = 4;
        [JsonPropertyName("descent")]
        public float Descent { get; set; } = 3;
        [JsonPropertyName("glyphs")]
        public ObservableCollection<SpriteFontGlyph> Glyphs { get; } = new ObservableCollection<SpriteFontGlyph>();

        public static SpriteFont LoadFromFile(string fileName)
        {
            string data = System.IO.File.ReadAllText(fileName);
            SpriteFont loadedFont = JsonSerializer.Deserialize<SpriteFont>(data);
            JsonDocument doc = JsonDocument.Parse(data);
            JsonElement glyphArray = doc.RootElement.GetProperty("glyphs");
            foreach (JsonElement glyph in glyphArray.EnumerateArray())
            {
                loadedFont.Glyphs.Add(JsonSerializer.Deserialize<SpriteFontGlyph>(glyph.GetRawText()));
            }
            return loadedFont;
        }
    }
}
