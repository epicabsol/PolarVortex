using System;
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

        public bool ShowCollision
        {
            get => ChildVisuals.Contains(CollisionVisual);
            set
            {
                if (value && !ChildVisuals.Contains(CollisionVisual))
                {
                    ChildVisuals.Add(CollisionVisual);
                    this.AddVisualChild(CollisionVisual);
                }
                else if (!value && ChildVisuals.Contains(CollisionVisual))
                {
                    ChildVisuals.Remove(CollisionVisual);
                    this.RemoveVisualChild(CollisionVisual);
                }
            }
        }

        private BitmapSource PaletteImage = null;
        private TilePalette Palette = null;
        private DrawingVisual TileVisual = new DrawingVisual();
        private DrawingVisual CollisionVisual = new DrawingVisual();
        private List<DrawingVisual> ChildVisuals = new List<DrawingVisual>();

        protected override int VisualChildrenCount => ChildVisuals.Count;
        protected override Visual GetVisualChild(int index) => ChildVisuals[index];

        protected override void OnInitialized(EventArgs e)
        {
            RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.NearestNeighbor);
            ChildVisuals.Add(TileVisual);
            //ChildVisuals.Add(CollisionVisual);
            base.OnInitialized(e);
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (this.World != null)
            {
                return new Size(this.Palette.TileSize * this.World.Width, this.Palette.TileSize * this.World.Height);
            }
            else
            {
                return Size.Empty;
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            if (this.World != null && this.Palette != null)
            {
                Brush backgroundBrush = (Brush)FindResource("BackgroundWellBrush");
                drawingContext.DrawRectangle(backgroundBrush, null, new Rect(0.0f, 0.0f, this.World.Width * this.Palette.TileSize - 0.0f, this.World.Height * this.Palette.TileSize - 0.0f));
            }
        }

        public void InvalidateTileVisual()
        {
            DrawingContext drawingContext = TileVisual.RenderOpen();
            if (this.World != null && this.Palette != null)
            {
                if (this.PaletteImage != null)
                {
                    for (int x = 0; x < this.World.Width; x++)
                    {
                        for (int y = 0; y < this.World.Height; y++)
                        {
                            if (this.World.Tiles[x, y].PaletteIndex != WorldTile.EmptyPaletteIndex)
                            {
                                CroppedBitmap tileSource = new CroppedBitmap(this.PaletteImage, new Int32Rect(Palette.TileSize * (this.World.Tiles[x, y].PaletteIndex % (PaletteImage.PixelWidth / Palette.TileSize)), Palette.TileSize * (this.World.Tiles[x, y].PaletteIndex / (PaletteImage.PixelHeight / Palette.TileSize)), Palette.TileSize, Palette.TileSize));
                                drawingContext.DrawImage(tileSource, new Rect(this.Palette.TileSize * x, this.Palette.TileSize * (World.Height - y - 1), this.Palette.TileSize, this.Palette.TileSize));
                            }
                        }
                    }
                }
            }

            drawingContext.Close();
            this.InvalidateVisual();
        }

        public void InvalidateCollisionVisual()
        {
            DrawingContext drawingContext = CollisionVisual.RenderOpen();

            if (this.World != null && this.Palette != null)
            {
                Brush collisionBrush = (Brush)FindResource("CollisionBrush");
                for (int x = 0; x < this.World.Width; x++)
                {
                    for (int y = 0; y < this.World.Height; y++)
                    {
                        if (this.World.Tiles[x, y].PaletteIndex != WorldTile.EmptyPaletteIndex)
                        {
                            drawingContext.DrawRectangle(collisionBrush, null, new Rect(this.Palette.TileSize * x, this.Palette.TileSize * (World.Height - y - 1), this.Palette.TileSize, this.Palette.TileSize));
                        }
                    }
                }
            }

            drawingContext.Close();
            this.InvalidateVisual();
        }

        private void ReloadPaletteImage()
        {
            if (!String.IsNullOrEmpty(this.BaseDirectory) && this.World != null)
            {
                this.Palette = TilePalette.LoadFromFile(System.IO.Path.Combine(this.BaseDirectory, this.World.PalettePath));
                this.PaletteImage = new BitmapImage(new Uri(System.IO.Path.Combine(this.BaseDirectory, this.Palette.TexturePath)));
                this.InvalidateCollisionVisual();
            }
            else
            {
                this.PaletteImage = null;
            }
            this.InvalidateTileVisual();
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
    }
}
