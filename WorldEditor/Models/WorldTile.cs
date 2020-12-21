using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace WorldEditor.Models
{
    public class WorldTile : INotifyPropertyChanged
    {
        private int _paletteIndex = -1;
        public int PaletteIndex
        {
            get => this._paletteIndex;
            set
            {
                this._paletteIndex = value;
                this.OnPropertyChanged(nameof(PaletteIndex));
            }
        }

        private bool _collides = false;
        public bool Collides
        {
            get => this._collides;
            set
            {
                this._collides = value;
                this.OnPropertyChanged(nameof(Collides));
            }
        }

        public WorldTile(int paletteIndex = -1, bool collides = false)
        {
            this._paletteIndex = paletteIndex;
            this._collides = collides;
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
