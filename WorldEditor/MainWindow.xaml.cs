using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WorldEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string BaseDirectory { get; }
        public Models.World CurrentWorld { get; }

        private float _scale = 1.0f;
        public float Scale
        {
            get => _scale;
            set
            {
                _scale = value;
                if (ZoomLabel != null)
                {
                    ZoomLabel.Text = $"{Scale}x";
                }
                if (WorldView != null)
                {
                    WorldView.LayoutTransform = new ScaleTransform(Scale, Scale);
                }
            }
        }

        public MainWindow(string baseDirectory, Models.World world)
        {
            this.BaseDirectory = baseDirectory;
            this.CurrentWorld = world;

            InitializeComponent();

            TileHoverRectangle.Width = WorldElement.Palette.TileSize;
            TileHoverRectangle.Height = WorldElement.Palette.TileSize;
        }

        private void ZoomInButton_Click(object sender, RoutedEventArgs e)
        {
            Scale = Scale * 2.0f;
        }

        private void ZoomOutButton_Click(object sender, RoutedEventArgs e)
        {
            Scale = Scale * 0.5f;
        }

        private void CollisionTool_Checked(object sender, RoutedEventArgs e)
        {
            WorldElement.ShowCollision = true;
        }

        private void CollisionTool_Unchecked(object sender, RoutedEventArgs e)
        {
            WorldElement.ShowCollision = false;
        }

        private void WorldElement_MouseEnter(object sender, MouseEventArgs e)
        {
            TileHoverRectangle.Visibility = Visibility.Visible;
        }

        private void WorldElement_MouseLeave(object sender, MouseEventArgs e)
        {
            TileHoverRectangle.Visibility = Visibility.Hidden;
        }

        private void WorldElement_MouseMove(object sender, MouseEventArgs e)
        {
            Canvas.SetLeft(TileHoverRectangle, Math.Floor(e.GetPosition(WorldElement).X / WorldElement.Palette.TileSize) * WorldElement.Palette.TileSize);
            Canvas.SetTop(TileHoverRectangle, Math.Floor(e.GetPosition(WorldElement).Y / WorldElement.Palette.TileSize) * WorldElement.Palette.TileSize);
        }
    }
}
