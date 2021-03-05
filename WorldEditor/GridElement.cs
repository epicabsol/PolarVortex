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

        public static DependencyProperty ShowBordersProperty = DependencyProperty.Register(nameof(ShowBorders), typeof(bool), typeof(GridElement), new PropertyMetadata(ShowBordersPropertyChanged));
        public bool ShowBorders
        {
            get => (bool)this.GetValue(ShowBordersProperty);
            set => this.SetValue(ShowBordersProperty, value);
        }

        private BitmapSource PaletteImage = null;
        public TilePalette Palette { get; private set; } = null;
        private DrawingVisual TileVisual = new DrawingVisual();
        private DrawingVisual CollisionVisual = new DrawingVisual();
        private DrawingVisual BordersVisual = new DrawingVisual();
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

        private const float BorderLineWidth = 1.0f / 16.0f;
        public void InvalidateBordersVisual()
        {
            DrawingContext drawingContext = this.BordersVisual.RenderOpen();

            if (this.Grid != null)
            {
                Brush foregroundPrimaryBrush = (Brush)this.FindResource("ForegroundPrimaryBrush");
                Pen foregroundPrimaryPen = new Pen(foregroundPrimaryBrush, BorderLineWidth);
                Brush foregroundSecondaryBrush = (Brush)this.FindResource("ForegroundSecondaryBrush");
                Pen foregroundSecondaryPen = new Pen(foregroundSecondaryBrush, BorderLineWidth);
                Brush hoverBrush = (Brush)this.FindResource("HoverBrush");
                Pen hoverPen = new Pen(hoverBrush, BorderLineWidth);

                for (int x = 1; x < this.Grid.Width; x++)
                {
                    drawingContext.DrawLine(hoverPen, new Point(x, BorderLineWidth), new Point(x, this.Grid.Height - BorderLineWidth));
                }
                for (int y = 1; y < this.Grid.Height; y++)
                {
                    drawingContext.DrawLine(hoverPen, new Point(BorderLineWidth, y), new Point(this.Grid.Width - BorderLineWidth, y));
                }

                drawingContext.DrawRectangle(null, foregroundPrimaryPen, new Rect(BorderLineWidth / 2.0f, BorderLineWidth / 2.0f, this.Grid.Width - BorderLineWidth, this.Grid.Height - BorderLineWidth));
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
            else if (e.PropertyName == nameof(Grid.Width) || e.PropertyName == nameof(Grid.Height))
            {
                this.InvalidateBordersVisual();
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

            this.InvalidateBordersVisual();

            if (e.OldValue is Grid oldGrid)
            {
                oldGrid.PropertyChanged -= Grid_PropertyChanged;
            }
            if (e.NewValue is Grid newGrid)
            {
                newGrid.PropertyChanged += Grid_PropertyChanged;
            }
        }

        protected virtual void OnBaseDirectoryChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            ReloadPaletteImage();
        }

        protected virtual void OnShowCollisionChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (this.ShowCollision && !ChildVisuals.Contains(this.CollisionVisual))
            {
                this.ChildVisuals.Add(this.CollisionVisual);
            }
            else if (!this.ShowCollision && this.ChildVisuals.Contains(this.CollisionVisual))
            {
                this.ChildVisuals.Remove(this.CollisionVisual);
            }
        }

        protected virtual void OnShowBordersChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (this.ShowBorders && !this.ChildVisuals.Contains(this.BordersVisual))
            {
                this.ChildVisuals.Add(this.BordersVisual);
            }
            else if (!this.ShowBorders && this.ChildVisuals.Contains(this.BordersVisual))
            {
                this.ChildVisuals.Remove(this.BordersVisual);
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

        private static void ShowBordersPropertyChanged(DependencyObject dp, DependencyPropertyChangedEventArgs e)
        {
            if (dp is GridElement gridElement)
            {
                gridElement.OnShowBordersChanged(gridElement, e);
            }
        }
    }
}
