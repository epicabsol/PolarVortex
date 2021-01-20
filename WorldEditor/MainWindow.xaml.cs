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
        public string WorldFilename { get; set; } = null; // `null` means unsaved

        // Tile tool
        public static DependencyProperty SelectedTileIndexProperty = DependencyProperty.Register(nameof(SelectedTileIndex), typeof(int), typeof(MainWindow));
        public int SelectedTileIndex
        {
            get => (int)this.GetValue(SelectedTileIndexProperty);
            set => this.SetValue(SelectedTileIndexProperty, value);
        }

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

        public MainWindow(string baseDirectory, Models.World world, string worldFilename)
        {
            this.BaseDirectory = baseDirectory;
            this.CurrentWorld = world;
            this.WorldFilename = worldFilename;

            InitializeComponent();

            TileHoverRectangle.Width = WorldElement.Palette.TileSize;
            TileHoverRectangle.Height = WorldElement.Palette.TileSize;
        }

        private bool SaveWorld()
        {
            if (WorldFilename == null)
            {
                return this.SaveWorldAs();
            }
            else
            {
#if !DEBUG
                try
                {
#endif
                    CurrentWorld.WriteToFile(WorldFilename);
                    return true;
#if !DEBUG
            }
                catch (Exception ex)
                {
                    return false;
                }
#endif
            }
        }

        private bool SaveWorldAs()
        {
            Microsoft.Win32.SaveFileDialog dialog = new Microsoft.Win32.SaveFileDialog();
            dialog.Filter = Models.World.DialogFilter;
            if (dialog.ShowDialog() ?? false)
            {
                this.WorldFilename = dialog.FileName;
                return this.SaveWorld();
            }
            else
            {
                return false;
            }
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

        private void WorldView_MouseEnter(object sender, MouseEventArgs e)
        {
            TileHoverRectangle.Visibility = Visibility.Visible;
        }

        private void WorldView_MouseLeave(object sender, MouseEventArgs e)
        {
            TileHoverRectangle.Visibility = Visibility.Hidden;
        }

        private void WorldView_MouseMove(object sender, MouseEventArgs e)
        {
            int tileX = (int)Math.Floor(e.GetPosition(WorldElement).X / WorldElement.Palette.TileSize);
            int tileY = CurrentWorld.Height - (int)Math.Floor(e.GetPosition(WorldElement).Y / WorldElement.Palette.TileSize) - 1;
            Canvas.SetLeft(TileHoverRectangle, tileX * WorldElement.Palette.TileSize);
            Canvas.SetTop(TileHoverRectangle, (CurrentWorld.Height - tileY - 1) * WorldElement.Palette.TileSize);

            if (TileToolButton.IsChecked ?? false)
            {
                if ((e.LeftButton == MouseButtonState.Pressed || e.RightButton == MouseButtonState.Pressed) && tileX >= 0 && tileX < WorldElement.World.Width && tileY >= 0 && tileY < WorldElement.World.Height)
                {
                    int newTile = (e.LeftButton == MouseButtonState.Pressed) ? SelectedTileIndex : -1;
                    if (CurrentWorld.Tiles[tileX, tileY].PaletteIndex != newTile)
                    {
                        CurrentWorld.Tiles[tileX, tileY].PaletteIndex = newTile;
                        WorldElement.InvalidateTileVisual();
                    }
                }
            }
            else if (CollisionToolButton.IsChecked ?? false)
            {
                if ((e.LeftButton == MouseButtonState.Pressed || e.RightButton == MouseButtonState.Pressed) && tileX >= 0 && tileX < WorldElement.World.Width && tileY >= 0 && tileY < WorldElement.World.Height)
                {
                    bool shouldCollide = e.LeftButton == MouseButtonState.Pressed;
                    if (CurrentWorld.Tiles[tileX, tileY].Collides != shouldCollide)
                    {
                        CurrentWorld.Tiles[tileX, tileY].Collides = shouldCollide;
                        WorldElement.InvalidateCollisionVisual();
                    }
                }
            }
            else if (EntityToolButton.IsChecked ?? false)
            {

            }
            else if (DecorToolButton.IsChecked ?? false)
            {

            }
        }

        private void WorldView_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int tileX = (int)Math.Floor(e.GetPosition(WorldElement).X / WorldElement.Palette.TileSize);
            int tileY = CurrentWorld.Height - (int)Math.Floor(e.GetPosition(WorldElement).Y / WorldElement.Palette.TileSize) - 1;
            if (TileToolButton.IsChecked ?? false)
            {
                CurrentWorld.Tiles[tileX, tileY].PaletteIndex = (e.ChangedButton == MouseButton.Left) ? SelectedTileIndex : -1;
                WorldElement.InvalidateTileVisual();
            }
            else if (CollisionToolButton.IsChecked ?? false)
            {
                CurrentWorld.Tiles[tileX, tileY].Collides = (e.ChangedButton == MouseButton.Left) ? true : false;
                WorldElement.InvalidateCollisionVisual();
            }
            else if (EntityToolButton.IsChecked ?? false)
            {

            }
            else if (DecorToolButton.IsChecked ?? false)
            {

            }
        }

        private void WorldView_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (TileToolButton.IsChecked ?? false)
            {

            }
            else if (CollisionToolButton.IsChecked ?? false)
            {

            }
            else if (EntityToolButton.IsChecked ?? false)
            {

            }
            else if (DecorToolButton.IsChecked ?? false)
            {

            }
        }

        private void SaveMenuItem_Click(object sender, RoutedEventArgs e)
        {
            this.SaveWorld();
        }

        private void SaveAsMenuItem_Click(object sender, RoutedEventArgs e)
        {
            this.SaveWorldAs();
        }
    }
}
