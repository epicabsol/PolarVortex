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

        public static DependencyProperty SelectedGridProperty = DependencyProperty.Register(nameof(SelectedGrid), typeof(Models.Grid), typeof(MainWindow));
        public Models.Grid SelectedGrid
        {
            get => (Models.Grid)this.GetValue(SelectedGridProperty);
            set => this.SetValue(SelectedGridProperty, value);
        }

        // Tile tool
        public static DependencyProperty SelectedTileIndexProperty = DependencyProperty.Register(nameof(SelectedTileIndex), typeof(int), typeof(MainWindow));
        public int SelectedTileIndex
        {
            get => (int)this.GetValue(SelectedTileIndexProperty);
            set => this.SetValue(SelectedTileIndexProperty, value);
        }

        // Collision tool
        public static DependencyProperty ShowCollisionProperty = DependencyProperty.Register(nameof(ShowCollision), typeof(bool), typeof(MainWindow));
        public bool ShowCollision
        {
            get => (bool)this.GetValue(ShowCollisionProperty);
            set => this.SetValue(ShowCollisionProperty, value);
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
            this.ShowCollision = true;
        }

        private void CollisionTool_Unchecked(object sender, RoutedEventArgs e)
        {
            this.ShowCollision = false;
        }

        private void GridElement_MouseEnter(object sender, MouseEventArgs e)
        {
            TileHoverRectangle.Visibility = Visibility.Visible;
        }

        private void GridElement_MouseLeave(object sender, MouseEventArgs e)
        {
            TileHoverRectangle.Visibility = Visibility.Hidden;
        }

        private bool IsLeftMouseDown = false;
        private bool IsRightMouseDown = false;
        private void GridElement_MouseMove(object sender, MouseEventArgs e)
        {
            GridElement element = (GridElement)sender;

            int tileX = (int)Math.Floor(e.GetPosition(element).X);
            int tileY = element.Grid.Height - (int)Math.Floor(e.GetPosition(element).Y) - 1;
            Canvas.SetLeft(TileHoverRectangle, tileX + element.Grid.X);
            Canvas.SetTop(TileHoverRectangle, (element.Grid.Height - (tileY + element.Grid.Y) - 1));

            if (TileToolButton.IsChecked ?? false)
            {
                if ((IsLeftMouseDown || IsRightMouseDown) && tileX >= 0 && tileX < element.Grid.Width && tileY >= 0 && tileY < element.Grid.Height)
                {
                    int newTile = IsLeftMouseDown ? SelectedTileIndex : -1;
                    int oldTile = element.Grid.Tiles[tileX, tileY].PaletteIndex;
                    if (oldTile != newTile)
                    {
                        this.UndoContext.DoAction(new Actions.SetTileAction(this.CurrentWorld.Grids.IndexOf(element.Grid), tileX, tileY, oldTile, newTile));
                    }
                }
            }
            else if (CollisionToolButton.IsChecked ?? false)
            {
                if ((IsLeftMouseDown || IsRightMouseDown) && tileX >= 0 && tileX < element.Grid.Width && tileY >= 0 && tileY < element.Grid.Height)
                {
                    bool oldCollision = element.Grid.Tiles[tileX, tileY].Collides;
                    bool newCollision = IsLeftMouseDown ? true : false;
                    if (newCollision != oldCollision)
                    {
                        this.UndoContext.DoAction(new Actions.SetTileCollisionAction(this.CurrentWorld.Grids.IndexOf(element.Grid), tileX, tileY, oldCollision, newCollision));
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

        private void GridElement_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                IsLeftMouseDown = true;
            }
            else if (e.ChangedButton == MouseButton.Right)
            {
                IsRightMouseDown = true;
            }

            GridElement element = (GridElement)sender;
            this.SelectedGrid = element.Grid;

            if (IsLeftMouseDown || IsRightMouseDown)
            {
                this.UndoContext.BeginGroup();
            }

            int tileX = (int)Math.Floor(e.GetPosition(element).X);
            int tileY = element.Grid.Height - (int)Math.Floor(e.GetPosition(element).Y) - 1;
            if (TileToolButton.IsChecked ?? false)
            {
                int newTile = IsLeftMouseDown ? SelectedTileIndex : -1;
                int oldTile = element.Grid.Tiles[tileX, tileY].PaletteIndex;
                if (oldTile != newTile)
                {
                    this.UndoContext.DoAction(new Actions.SetTileAction(this.CurrentWorld.Grids.IndexOf(element.Grid), tileX, tileY, oldTile, newTile));
                }
            }
            else if (CollisionToolButton.IsChecked ?? false)
            {
                bool oldCollision = element.Grid.Tiles[tileX, tileY].Collides;
                bool newCollision = IsLeftMouseDown ? true : false;
                if (newCollision != oldCollision)
                {
                    this.UndoContext.DoAction(new Actions.SetTileCollisionAction(this.CurrentWorld.Grids.IndexOf(element.Grid), tileX, tileY, oldCollision, newCollision));
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

        private void GridElement_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left && IsLeftMouseDown)
            {
                IsLeftMouseDown = false;
            }
            else if (e.ChangedButton == MouseButton.Right && IsRightMouseDown)
            {
                IsRightMouseDown = false;
            }
            else
            {
                // Sometimes we get a MouseUp when the click that closed the previous window is released, but before the user has pressed down on this window.
                // Ignore those.
                return;
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

            this.UndoContext.EndGroup();
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
        }

        private void UndoCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.UndoContext.CanUndo;
        }

        private void RedoCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            this.UndoContext.RedoAction();
        }

        private void RedoCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.UndoContext.CanRedo;
        }

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
        #endregion
    }
}
