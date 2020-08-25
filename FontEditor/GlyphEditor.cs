using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows;
using System.Windows.Forms.Design.Behavior;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace FontEditor
{
    public class GlyphEditor : FrameworkElement
    {
        public static DependencyProperty FontProperty = DependencyProperty.Register(nameof(Font), typeof(SpriteFont), typeof(GlyphEditor), new PropertyMetadata(OnPropertyChange));
        public SpriteFont Font
        {
            get => (SpriteFont)GetValue(FontProperty);
            set => SetValue(FontProperty, value);
        }

        public static DependencyProperty GlyphProperty = DependencyProperty.Register(nameof(Glyph), typeof(SpriteFontGlyph), typeof(GlyphEditor), new PropertyMetadata(OnPropertyChange));
        public SpriteFontGlyph Glyph
        {
            get => (SpriteFontGlyph)GetValue(GlyphProperty);
            set => SetValue(FontProperty, value);
        }

        public static DependencyProperty ScaleProperty = DependencyProperty.Register(nameof(Scale), typeof(float), typeof(GlyphEditor), new PropertyMetadata((float)1, OnPropertyChange));
        public float Scale
        {
            get => (float)GetValue(ScaleProperty);
            set => SetValue(ScaleProperty, value);
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            SolidColorBrush backgroundBrush = new SolidColorBrush(System.Windows.Media.Color.FromRgb(40, 40, 40));
            drawingContext.DrawRectangle(backgroundBrush, null, new Rect(0, 0, this.RenderSize.Width, this.RenderSize.Height));
            if (this.Font != null && this.Font.TexturePath?.Length > 0)
            {
                BitmapImage image = new BitmapImage();
                image.BeginInit();
                image.UriSource = new Uri(System.IO.Path.Combine(MainWindow.AssetDirectory, Font.TexturePath));
                
                image.EndInit();
                RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.NearestNeighbor);
                ImageBrush brush = new ImageBrush(image);
                
                drawingContext.DrawRectangle(brush, null, new Rect(0, 0, image.PixelWidth * Scale, image.PixelHeight * Scale));
            }

        }

        protected override System.Windows.Size MeasureOverride(System.Windows.Size availableSize)
        {
            if (this.Font != null && this.Font.TexturePath?.Length > 0)
            {
                BitmapImage image = new BitmapImage();
                image.BeginInit();
                image.UriSource = new Uri(System.IO.Path.Combine(MainWindow.AssetDirectory, Font.TexturePath));
                image.EndInit();
                return new System.Windows.Size((double)image.PixelWidth * Scale, (double)image.PixelHeight * Scale);
            }
            return base.MeasureOverride(availableSize);
        }

        private static void OnPropertyChange(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            if (obj is GlyphEditor editor)
            {
                editor.InvalidateVisual();
                editor.InvalidateMeasure();
            }
        }
    }
}
