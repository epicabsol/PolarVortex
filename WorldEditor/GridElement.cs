using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using WorldEditor.Models;

namespace WorldEditor
{
    public class GridElement : FrameworkElement
    {
        public static DependencyProperty GridProperty = DependencyProperty.Register(nameof(Grid), typeof(Grid), typeof(GridElement), new PropertyMetadata(GridPropertyChanged));
        public Grid Grid
        {
            get => (Grid)this.GetValue(GridProperty);
            set => this.SetValue(GridProperty, value);
        }

        public static DependencyProperty BaseDirectoryProperty = DependencyProperty.Register(nameof(BaseDirectory), typeof(string), typeof(GridElement), new PropertyMetadata(BaseDirectoryPropertyChanged));
        public string BaseDirectory
        {
            get => (string)this.GetValue(BaseDirectoryProperty);
            set => this.SetValue(BaseDirectoryProperty, value);
        }

        public static DependencyProperty ShowCollisionProperty = DependencyProperty.Register(nameof(ShowCollision), typeof(bool), typeof(GridElement), new PropertyMetadata(ShowCollisionPropertyChanged));
        public bool ShowCollision
        {
            get => (bool)this.GetValue(ShowCollisionProperty);
            set => this.SetValue(ShowCollisionProperty, value);
        }

        private BitmapSource PaletteImage = null;
        public TilePalette Palette { get; private set; } = null;
        private DrawingVisual TileVisual = new DrawingVisual();
        private DrawingVisual CollisionVisual = new DrawingVisual();
        private VisualCollection ChildVisuals;

        protected override int VisualChildrenCount => ChildVisuals?.Count ?? 0;
        protected override Visual GetVisualChild(int index) => ChildVisuals[index];

        protected override void OnInitialized(EventArgs e)
        {
            this.ChildVisuals = new VisualCollection(this);
            RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.NearestNeighbor);
            ChildVisuals.Add(TileVisual);
            base.OnInitialized(e);
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (this.Grid != null)
            {
                return new Size(this.Grid.Width, this.Grid.Height);
            }
            else
            {
                return Size.Empty;
            }
        }

        protected override HitTestResult HitTestCore(PointHitTestParameters hitTestParameters)
        {
            if (hitTestParameters.HitPoint.X >= 0
                && hitTestParameters.HitPoint.X < this.ActualWidth
                && hitTestParameters.HitPoint.Y >= 0
                && hitTestParameters.HitPoint.Y < this.ActualHeight)
            {
                return new PointHitTestResult(this, hitTestParameters.HitPoint);
            }
            else
            {
                return new PointHitTestResult(null, hitTestParameters.HitPoint);
            }
        }

        public void InvalidateTileVisual()
        {
            DrawingContext drawingContext = TileVisual.RenderOpen();
            if (this.Grid != null && this.Palette != null)
            {
                if (this.PaletteImage != null)
                {
                    for (int x = 0; x < this.Grid.Width; x++)
                    {
                        for (int y = 0; y < this.Grid.Height; y++)
                        {
                            if (this.Grid.Tiles[x, y].PaletteIndex != WorldTile.EmptyPaletteIndex)
                            {
                                CroppedBitmap tileSource = new CroppedBitmap(this.PaletteImage, new Int32Rect(Palette.TileSize * (this.Grid.Tiles[x, y].PaletteIndex % (PaletteImage.PixelWidth / Palette.TileSize)), Palette.TileSize * (this.Grid.Tiles[x, y].PaletteIndex / (PaletteImage.PixelHeight / Palette.TileSize)), Palette.TileSize, Palette.TileSize));
                                drawingContext.DrawImage(tileSource, new Rect(x, (this.Grid.Height - y - 1), 1.0f, 1.0f));
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

            if (this.Grid != null && this.Palette != null)
            {
                Brush collisionBrush = (Brush)FindResource("CollisionBrush");
                for (int x = 0; x < this.Grid.Width; x++)
                {
                    for (int y = 0; y < this.Grid.Height; y++)
                    {
                        if (this.Grid.Tiles[x, y].Collides)
                        {
                            drawingContext.DrawRectangle(collisionBrush, null, new Rect(x, (this.Grid.Height - y - 1), 1.0f, 1.0f));
                        }
                    }
                }
            }

            drawingContext.Close();
            this.InvalidateVisual();
        }

        private void ReloadPaletteImage()
        {
            if (!String.IsNullOrEmpty(this.BaseDirectory) && this.Grid != null)
            {
                this.Palette = TilePalette.LoadFromFile(System.IO.Path.Combine(this.BaseDirectory, this.Grid.PalettePath));
                this.PaletteImage = new BitmapImage(new Uri(System.IO.Path.Combine(this.BaseDirectory, this.Palette.TexturePath)));
                this.InvalidateCollisionVisual();
            }
            else
            {
                this.PaletteImage = null;
            }
            this.InvalidateTileVisual();
        }

        private void Grid_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(Grid.Tiles))
            {
                this.InvalidateTileVisual();
                this.InvalidateCollisionVisual();
            }
            else if (e.PropertyName == nameof(Grid.PalettePath))
            {
                this.ReloadPaletteImage();
            }
        }

        protected virtual void OnGridChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            InvalidateMeasure();

            ReloadPaletteImage();

            this.Grid.PropertyChanged += Grid_PropertyChanged;
        }

        protected virtual void OnBaseDirectoryChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            ReloadPaletteImage();
        }

        protected virtual void OnShowCollisionChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (this.ShowCollision && !ChildVisuals.Contains(CollisionVisual))
            {
                ChildVisuals.Add(CollisionVisual);
            }
            else if (!this.ShowCollision && ChildVisuals.Contains(CollisionVisual))
            {
                ChildVisuals.Remove(CollisionVisual);
            }
        }

        private static void GridPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is GridElement gridElement)
            {
                gridElement.OnGridChanged(gridElement, e);
            }
        }

        private static void BaseDirectoryPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is GridElement gridElement)
            {
                gridElement.OnBaseDirectoryChanged(gridElement, e);
            }
        }

        private static void ShowCollisionPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is GridElement gridElement)
            {
                gridElement.OnShowCollisionChanged(gridElement, e);
            }
        }
    }
}
