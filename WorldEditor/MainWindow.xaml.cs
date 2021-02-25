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

        public Actions.UndoContext UndoContext { get; }

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
            this.UndoContext = new Actions.UndoContext(world);

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

        private bool IsLeftMouseDown = false;
        private bool IsRightMouseDown = false;
        private void WorldView_MouseMove(object sender, MouseEventArgs e)
        {
            int tileX = (int)Math.Floor(e.GetPosition(WorldElement).X / WorldElement.Palette.TileSize);
            int tileY = CurrentWorld.Height - (int)Math.Floor(e.GetPosition(WorldElement).Y / WorldElement.Palette.TileSize) - 1;
            Canvas.SetLeft(TileHoverRectangle, tileX * WorldElement.Palette.TileSize);
            Canvas.SetTop(TileHoverRectangle, (CurrentWorld.Height - tileY - 1) * WorldElement.Palette.TileSize);

            if (TileToolButton.IsChecked ?? false)
            {
                if ((IsLeftMouseDown || IsRightMouseDown) && tileX >= 0 && tileX < WorldElement.World.Width && tileY >= 0 && tileY < WorldElement.World.Height)
                {
                    int newTile = (IsLeftMouseDown) ? SelectedTileIndex : -1;
                    int oldTile = CurrentWorld.Tiles[tileX, tileY].PaletteIndex;
                    if (oldTile != newTile)
                    {
                        //CurrentWorld.Tiles[tileX, tileY].PaletteIndex = newTile;
                        this.UndoContext.ExecuteAction(new Actions.SetTileAction(tileX, tileY, oldTile, newTile));
                        
                        // HACK
                        WorldElement.InvalidateTileVisual();
                    }
                }
            }
            else if (CollisionToolButton.IsChecked ?? false)
            {
                if ((IsLeftMouseDown || IsRightMouseDown) && tileX >= 0 && tileX < WorldElement.World.Width && tileY >= 0 && tileY < WorldElement.World.Height)
                {
                    bool shouldCollide = IsLeftMouseDown;
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
            else if (GridsToolButton.IsChecked ?? false)
            {

            }
            else if (BackgroundToolButton.IsChecked ?? false)
            {

            }
        }

        private void WorldView_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                IsLeftMouseDown = true;
            }
            else if (e.ChangedButton == MouseButton.Right)
            {
                IsRightMouseDown = true;
            }

            int tileX = (int)Math.Floor(e.GetPosition(WorldElement).X / WorldElement.Palette.TileSize);
            int tileY = CurrentWorld.Height - (int)Math.Floor(e.GetPosition(WorldElement).Y / WorldElement.Palette.TileSize) - 1;
            if (TileToolButton.IsChecked ?? false)
            {
                int newTile = (IsLeftMouseDown) ? SelectedTileIndex : -1;
                int oldTile = CurrentWorld.Tiles[tileX, tileY].PaletteIndex;
                if (oldTile != newTile)
                {
                    //CurrentWorld.Tiles[tileX, tileY].PaletteIndex = newTile;
                    this.UndoContext.ExecuteAction(new Actions.SetTileAction(tileX, tileY, oldTile, newTile));

                    // HACK
                    WorldElement.InvalidateTileVisual();
                }
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
            else if (GridsToolButton.IsChecked ?? false)
            {

            }
            else if (BackgroundToolButton.IsChecked ?? false)
            {

            }
        }

        private void WorldView_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                IsLeftMouseDown = false;
            }
            else if (e.ChangedButton == MouseButton.Right)
            {
                IsRightMouseDown = false;
            }

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
            else if (GridsToolButton.IsChecked ?? false)
            {

            }
            else if (BackgroundToolButton.IsChecked ?? false)
            {

            }
        }

        private void SaveAsMenuItem_Click(object sender, RoutedEventArgs e)
        {
            
        }

        #region Commands
        private void SaveCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            this.SaveWorld();
        }

        private void SaveCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void UndoCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            this.UndoContext.UndoAction();
            WorldElement.InvalidateTileVisual(); // HACK: Do this automatically when necessary
        }

        private void UndoCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.UndoContext.CanUndo;
        }

        private void RedoCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            this.UndoContext.RedoAction();
            WorldElement.InvalidateTileVisual(); // HACK: Do this automatically when necessary
        }

        private void RedoCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.UndoContext.CanRedo;
        }
        #endregion

        private void NewCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            // TODO: Implement!
        }

        private void NewCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void SaveAsCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            this.SaveWorldAs();
        }

        private void SaveAsCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }
    }
}
