using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WorldEditor.Models;

namespace WorldEditor
{
    /// <summary>
    /// Interaction logic for TilePicker.xaml
    /// </summary>
    public partial class TilePicker : UserControl
    {
        public static readonly DependencyProperty PalettePathProperty = DependencyProperty.Register(nameof(PalettePath), typeof(string), typeof(TilePicker), new PropertyMetadata("", PalettePathPropertyChanged));
        public string PalettePath
        {
            get => (string)this.GetValue(PalettePathProperty);
            set => this.SetValue(PalettePathProperty, value);
        }

        public static readonly DependencyProperty BaseDirectoryProperty = DependencyProperty.Register(nameof(BaseDirectory), typeof(string), typeof(TilePicker), new PropertyMetadata("", BaseDirectoryPropertyChanged));
        public string BaseDirectory
        {
            get => (string)this.GetValue(BaseDirectoryProperty);
            set => this.SetValue(BaseDirectoryProperty, value);
        }

        public static readonly DependencyProperty SelectedIndexProperty = DependencyProperty.Register(nameof(SelectedIndex), typeof(int), typeof(TilePicker), new PropertyMetadata(0, SelectedIndexPropertyChanged));
        public int SelectedIndex
        {
            get => (int)this.GetValue(SelectedIndexProperty);
            set => this.SetValue(SelectedIndexProperty, value);
        }

        private TilePalette Palette { get; set; } = null;
        private BitmapSource PaletteImageSource { get; set; } = null;

        public TilePicker()
        {
            InitializeComponent();
            UpdatePalette();
            UpdateSelection();
        }

        private void UpdatePalette()
        {
            if (!String.IsNullOrEmpty(PalettePath) && !String.IsNullOrEmpty(BaseDirectory))
            {
                Palette = TilePalette.LoadFromFile(System.IO.Path.Combine(BaseDirectory, PalettePath));
                PaletteImageSource = new BitmapImage(new Uri(System.IO.Path.Combine(BaseDirectory, Palette.TexturePath)));
                PaletteImage.Source = PaletteImageSource;
            }
            else
            {
                Palette = null;
                PaletteImageSource = null;
                PaletteImage.Source = null;
            }
            UpdateSelection();
        }

        private void UpdateSelection()
        {
            if (Palette != null && PaletteImage.ActualHeight > 0 && PaletteImage.ActualWidth > 0)
            {
                SelectionRectangle.Visibility = Visibility.Visible;
                double realTileSize = PaletteImage.ActualWidth / (PaletteImageSource.PixelWidth / Palette.TileSize);
                SelectionRectangle.Width = realTileSize;
                SelectionRectangle.Height = realTileSize;
                HoverRectangle.Width = realTileSize;
                HoverRectangle.Height = realTileSize;
                int tileX = SelectedIndex % (PaletteImageSource.PixelWidth / Palette.TileSize);
                int tileY = (int)Math.Floor((float)SelectedIndex / (PaletteImageSource.PixelWidth / Palette.TileSize));
                Canvas.SetLeft(SelectionRectangle, tileX * realTileSize);
                Canvas.SetTop(SelectionRectangle, tileY * realTileSize);
            }
            else
            {
                SelectionRectangle.Visibility = Visibility.Hidden;
            }
        }

        private void UserControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            UpdateSelection();
        }

        private void PaletteImage_MouseEnter(object sender, MouseEventArgs e)
        {
            HoverRectangle.Visibility = Visibility.Visible;
        }

        private void PaletteImage_MouseLeave(object sender, MouseEventArgs e)
        {
            HoverRectangle.Visibility = Visibility.Hidden;
        }

        private void PaletteImage_MouseMove(object sender, MouseEventArgs e)
        {
            double realTileSize = PaletteImage.ActualWidth / (PaletteImageSource.PixelWidth / Palette.TileSize);
            Canvas.SetLeft(HoverRectangle, Math.Floor(e.GetPosition(PaletteImage).X / realTileSize) * realTileSize);
            Canvas.SetTop(HoverRectangle, Math.Floor(e.GetPosition(PaletteImage).Y / realTileSize) * realTileSize);
        }

        private void PaletteImage_MouseDown(object sender, MouseButtonEventArgs e)
        {
            double realTileSize = PaletteImage.ActualWidth / (PaletteImageSource.PixelWidth / Palette.TileSize);
            int tileX = (int)Math.Floor(e.GetPosition(PaletteImage).X / realTileSize);
            int tileY = (int)Math.Floor(e.GetPosition(PaletteImage).Y / realTileSize);
            this.SelectedIndex = tileY * (PaletteImageSource.PixelWidth / Palette.TileSize) + tileX;
        }

        protected virtual void OnPalettePathChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            UpdatePalette();
        }

        protected virtual void OnBaseDirectoryChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            UpdatePalette();
        }

        protected virtual void OnSelectedIndexChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            UpdateSelection();
        }

        private static void PalettePathPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is TilePicker picker)
            {
                picker.OnPalettePathChanged(picker, e);
            }
        }

        private static void BaseDirectoryPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is TilePicker picker)
            {
                picker.OnBaseDirectoryChanged(picker, e);
            }
        }

        private static void SelectedIndexPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is TilePicker picker)
            {
                picker.OnSelectedIndexChanged(picker, e);
            }
        }
    }
}
