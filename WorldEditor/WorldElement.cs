﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using WorldEditor.Models;

namespace WorldEditor
{
    public class WorldElement : FrameworkElement
    {
        public static DependencyProperty WorldProperty = DependencyProperty.Register(nameof(World), typeof(World), typeof(WorldElement), new PropertyMetadata(WorldPropertyChanged));
        public World World
        {
            get => (World)this.GetValue(WorldProperty);
            set => this.SetValue(WorldProperty, value);
        }

        public static DependencyProperty BaseDirectoryProperty = DependencyProperty.Register(nameof(BaseDirectory), typeof(string), typeof(WorldElement), new PropertyMetadata(BaseDirectoryPropertyChanged));
        public string BaseDirectory
        {
            get => (string)this.GetValue(BaseDirectoryProperty);
            set => this.SetValue(BaseDirectoryProperty, value);
        }

        public static DependencyProperty ScaleProperty = DependencyProperty.Register(nameof(Scale), typeof(float), typeof(WorldElement), new PropertyMetadata(1.0f, ScalePropertyChanged));
        public float Scale
        {
            get => (float)this.GetValue(ScaleProperty);
            set => this.SetValue(ScaleProperty, value);
        }
        public event EventHandler<float> ScaleChanged;

        private BitmapSource PaletteImage = null;
        private TilePalette Palette = null;

        protected override void OnInitialized(EventArgs e)
        {
            RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.NearestNeighbor);
            base.OnInitialized(e);
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (this.World != null)
            {
                return new Size(this.World.Width * Scale, this.World.Height * Scale);
            }
            else
            {
                return Size.Empty;
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            if (this.World != null)
            {
                Brush backgroundBrush = (Brush)FindResource("BackgroundBrush");
                drawingContext.DrawRectangle(backgroundBrush, null, new Rect(0.0f, 0.0f, this.World.Width * this.Palette.TileSize * Scale - 0.0f, this.World.Height * this.Palette.TileSize * Scale - 0.0f));

                if (this.PaletteImage != null)
                {
                    for (int x = 0; x < this.World.Width; x++)
                    {
                        for (int y = 0; y < this.World.Height; y++)
                        {
                            if (this.World.Tiles[x, y].PaletteIndex != WorldTile.EmptyPaletteIndex)
                            {
                                CroppedBitmap tileSource = new CroppedBitmap(this.PaletteImage, new Int32Rect(Palette.TileSize * (this.World.Tiles[x, y].PaletteIndex % (PaletteImage.PixelWidth / Palette.TileSize)), Palette.TileSize * (this.World.Tiles[x, y].PaletteIndex / (PaletteImage.PixelHeight / Palette.TileSize)), Palette.TileSize, Palette.TileSize));
                                drawingContext.DrawImage(tileSource, new Rect(this.Palette.TileSize * x * Scale, this.Palette.TileSize * (World.Height - y - 1) * Scale, this.Palette.TileSize * Scale, this.Palette.TileSize * Scale));
                            }
                        }
                    }
                }
            }
        }

        private void ReloadPaletteImage()
        {
            if (!String.IsNullOrEmpty(this.BaseDirectory) && this.World != null)
            {
                this.Palette = TilePalette.LoadFromFile(System.IO.Path.Combine(this.BaseDirectory, this.World.PalettePath));
                this.PaletteImage = new BitmapImage(new Uri(System.IO.Path.Combine(this.BaseDirectory, this.Palette.TexturePath)));
                //this.Scale = this.Palette.TileSize * 2.0f;
            }
            else
            {
                this.PaletteImage = null;
            }
            this.InvalidateVisual();
        }

        protected virtual void OnWorldChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            InvalidateMeasure();

            ReloadPaletteImage();
        }

        protected virtual void OnBaseDirectoryChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            ReloadPaletteImage();
        }

        protected virtual void OnScaleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            InvalidateMeasure();
            InvalidateVisual();
            ScaleChanged?.Invoke(sender, Scale);
        }

        private static void WorldPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is WorldElement worldElement)
            {
                worldElement.OnWorldChanged(worldElement, e);
            }
        }

        private static void BaseDirectoryPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is WorldElement worldElement)
            {
                worldElement.OnBaseDirectoryChanged(worldElement, e);
            }
        }

        private static void ScalePropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is WorldElement worldElement)
            {
                worldElement.OnScaleChanged(worldElement, e);
            }
        }
    }
}
