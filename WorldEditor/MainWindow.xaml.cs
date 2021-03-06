using System;
using System.Collections.Generic;
using System.Globalization;
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
    [ValueConversion(typeof(float), typeof(double))]
    public class GridYConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if (values.Length == 2 && values[0] is int y && values[1] is int height)
            {
                return (double)(-y - height);
            }
            else
            {
                return DependencyProperty.UnsetValue;
            }
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private class GridResizeConfig
        {
            public Models.Grid Grid { get; }
            public Models.WorldTile[,] StartTiles { get; }
            public int XFactor { get; }
            public int YFactor { get; }
            public int WidthFactor { get; }
            public int HeightFactor { get; }

            public GridResizeConfig(Models.Grid grid, int xFactor, int yFactor, int widthFactor, int heightFactor)
            {
                this.Grid = grid;
                this.StartTiles = grid.CloneTiles();
                this.XFactor = xFactor;
                this.YFactor = yFactor;
                this.WidthFactor = widthFactor;
                this.HeightFactor = heightFactor;
            }
        }

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

        // Grids tool
        public static DependencyProperty ShowBordersProperty = DependencyProperty.Register(nameof(ShowBorders), typeof(bool), typeof(MainWindow));
        public bool ShowBorders
        {
            get => (bool)this.GetValue(ShowBordersProperty);
            set => this.SetValue(ShowBordersProperty, value);
        }

        private float _scale = 1.0f;
        public float Scale
        {
            get => _scale;
            set
            {
                _scale = value;
                if (this.ZoomLabel != null)
                {
                    this.ZoomLabel.Text = $"{this.Scale}x";
                }
                if (WorldView != null)
                {
                    this.UpdateViewTransform();
                }
            }
        }
        private Vector ViewOffset = new Vector();

        public MainWindow(string baseDirectory, Models.World world, string worldFilename)
        {
            this.BaseDirectory = baseDirectory;
            this.CurrentWorld = world;
            this.WorldFilename = worldFilename;
            this.UndoContext = new Actions.UndoContext(world);

            InitializeComponent();

            this.ViewOffset.Y = 750.0f; // HACK: We should correctly focus on the center of the world bounds, not a hardcoded point.
            this.Scale = 32.0f;
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

        private void UpdateViewTransform()
        {
            TransformGroup viewTransform = new TransformGroup();
            viewTransform.Children.Add(new ScaleTransform(this.Scale, this.Scale));
            viewTransform.Children.Add(new TranslateTransform((int)this.ViewOffset.X, (int)this.ViewOffset.Y));
            WorldView.RenderTransform = viewTransform;
        }

        private void UpdateOffsetForScaleChange(float oldScale, float newScale, Point screenFocalPoint)
        {
            float focalX = (float)(screenFocalPoint.X - ViewOffset.X) / oldScale;
            float focalY = (float)(screenFocalPoint.Y - ViewOffset.Y) / oldScale;
            ViewOffset.X = -1.0f * (focalX * newScale - (float)screenFocalPoint.X);
            ViewOffset.Y = -1.0f * (focalY * newScale - (float)screenFocalPoint.Y);
            if (WorldView != null)
            {
                this.UpdateViewTransform();
            }
        }

        private void ZoomInButton_Click(object sender, RoutedEventArgs e)
        {
            this.UpdateOffsetForScaleChange(this.Scale, this.Scale * 2.0f, new Point((float)WorldViewport.ActualWidth / 2.0f, (float)WorldViewport.ActualHeight / 2.0f));
            this.Scale *= 2.0f;
        }

        private void ZoomOutButton_Click(object sender, RoutedEventArgs e)
        {
            this.UpdateOffsetForScaleChange(this.Scale, this.Scale * 0.5f, new Point((float)WorldViewport.ActualWidth / 2.0f, (float)WorldViewport.ActualHeight / 2.0f));
            this.Scale *= 0.5f;
        }

        private void CollisionToolButton_Checked(object sender, RoutedEventArgs e)
        {
            this.ShowCollision = true;
        }

        private void CollisionToolButton_Unchecked(object sender, RoutedEventArgs e)
        {
            this.ShowCollision = false;
        }

        private void GridsToolButton_Checked(object sender, RoutedEventArgs e)
        {
            this.ShowBorders = true;
        }

        private void GridsToolButton_Unchecked(object sender, RoutedEventArgs e)
        {
            this.ShowBorders = false;
        }

        private void GridElement_MouseEnter(object sender, MouseEventArgs e)
        {
            GridElement element = (GridElement)sender;

            this.SelectedGrid = element.Grid;

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
            Canvas.SetTop(TileHoverRectangle, -tileY - element.Grid.Y - 1);

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

        private bool Panning = false;
        private Point LastMouse = new Point();
        private void WorldViewport_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Middle)
            {
                this.Panning = true;
                LastMouse = e.GetPosition(WorldViewport);
                WorldViewport.CaptureMouse();
                e.Handled = true;
            }
        }

        private void WorldViewport_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (this.Panning)
            {
                Point point = e.GetPosition(WorldViewport);
                if (point != LastMouse)
                {
                    this.ViewOffset.X += point.X - LastMouse.X;
                    this.ViewOffset.Y += point.Y - LastMouse.Y;
                    this.UpdateViewTransform();

                }
                LastMouse = point;
                e.Handled = true;
            }
        }

        private void WorldViewport_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Middle && this.Panning)
            {
                this.Panning = false;
                WorldViewport.ReleaseMouseCapture();
                e.Handled = true;
            }
        }

        private float ZoomAccumulator = 0.0f;
        private void WorldViewport_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            this.ZoomAccumulator += e.Delta / 120.0f;
            Point point = e.GetPosition(WorldViewport);
            while (this.ZoomAccumulator >= 1.0f)
            {
                this.UpdateOffsetForScaleChange(this.Scale, this.Scale * 2.0f, point);
                this.Scale *= 2.0f;
                this.ZoomAccumulator -= 1.0f;
            }
            while (this.ZoomAccumulator <= -1.0f)
            {
                this.UpdateOffsetForScaleChange(this.Scale, this.Scale * 0.5f, point);
                this.Scale *= 0.5f;
                this.ZoomAccumulator += 1.0f;
            }
            e.Handled = true;
        }

        #region Grid moving and resizing
        private Point MoveReferencePoint;
        private Models.Grid DragGrid;
        private int DragStartX;
        private int DragStartY;
        private int DragStartWidth;
        private int DragStartHeight;
        private void MoveRectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Rectangle moveRectangle && moveRectangle.DataContext is Models.Grid grid)
            {
                this.MoveReferencePoint = e.GetPosition(moveRectangle);
                moveRectangle.CaptureMouse();
                this.DragGrid = grid;
                this.DragStartX = grid.X;
                this.DragStartY = grid.Y;
            }
        }

        private void MoveRectangle_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Source is Rectangle moveRectangle && moveRectangle.DataContext is Models.Grid grid && grid == this.DragGrid)
            {
                Point point = e.GetPosition(moveRectangle);
                Vector delta = new Vector(point.X - this.MoveReferencePoint.X, point.Y - this.MoveReferencePoint.Y);
                int dX = (int)Math.Truncate(delta.X);
                int dY = (int)Math.Truncate(delta.Y);
                if (grid.X != this.DragStartX + dX || grid.Y != this.DragStartY - dY)
                {
                    MoveReferencePoint.X += (grid.X - (this.DragStartX + dX));
                    MoveReferencePoint.Y -= (grid.Y - (this.DragStartY - dY));
                    grid.X = this.DragStartX + dX;
                    grid.Y = this.DragStartY - dY;
                }
            }
        }

        private void MoveRectangle_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Rectangle moveRectangle && moveRectangle.DataContext is Models.Grid grid && grid == this.DragGrid)
            {
                moveRectangle.ReleaseMouseCapture();

                if (grid.X != this.DragStartX || grid.Y != this.DragStartY)
                {
                    this.UndoContext.DoAction(new Actions.MoveGridAction(this.CurrentWorld.Grids.IndexOf(grid), grid.X - this.DragStartX, grid.Y - this.DragStartY), false);
                }
            }
            this.DragGrid = null;
        }

        private GridResizeConfig CurrentResize = null;
        #endregion

        private void GridTopRightCorner_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Polyline corner && corner.DataContext is Models.Grid grid)
            {
                GridCorner_MouseDown(corner, grid, 0, 0, 1, 1, e);
            }
        }

        private void GridTopLeftCorner_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Polyline corner && corner.DataContext is Models.Grid grid)
            {
                GridCorner_MouseDown(corner, grid, 1, 0, -1, 1, e);
            }
        }

        private void GridBottomLeftCorner_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Polyline corner && corner.DataContext is Models.Grid grid)
            {
                GridCorner_MouseDown(corner, grid, 1, 1, -1, -1, e);
            }
        }

        private void GridBottomRightCorner_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Polyline corner && corner.DataContext is Models.Grid grid)
            {
                GridCorner_MouseDown(corner, grid, 0, 1, 1, -1, e);
            }
        }

        private void GridCorner_MouseDown(Polyline corner, Models.Grid grid, int xFactor, int yFactor, int widthFactor, int heightFactor, MouseButtonEventArgs e)
        {
            this.CurrentResize = new GridResizeConfig(grid, xFactor, yFactor, widthFactor, heightFactor);
            this.MoveReferencePoint = e.GetPosition(corner);
            corner.CaptureMouse();
            this.DragStartX = grid.X;
            this.DragStartY = grid.Y;
            this.DragStartWidth = grid.Width;
            this.DragStartHeight = grid.Height;
        }

        private void GridCorner_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Source is Polyline corner && corner.DataContext is Models.Grid grid && grid == this.CurrentResize?.Grid)
            {
                Point point = e.GetPosition(corner);
                Vector delta = new Vector(point.X - this.MoveReferencePoint.X, point.Y - this.MoveReferencePoint.Y);
                int dX = (int)Math.Truncate(delta.X);
                int dY = (int)Math.Truncate(delta.Y);

                int newX = this.DragStartX + (dX * this.CurrentResize.XFactor);
                int newY = this.DragStartY - (dY * this.CurrentResize.YFactor);
                int newWidth = this.DragStartWidth + (dX * this.CurrentResize.WidthFactor);
                int newHeight = this.DragStartHeight - (dY * this.CurrentResize.HeightFactor);
                if (grid.X != newX || grid.Y != newY || grid.Width != newWidth || grid.Height != newHeight)
                {
                    MoveReferencePoint.X += (grid.X - newX) * this.CurrentResize.XFactor + (grid.Width - newWidth) * (this.CurrentResize.XFactor + this.CurrentResize.WidthFactor);
                    MoveReferencePoint.Y -= (grid.Y - newY) * this.CurrentResize.YFactor + (grid.Height - newHeight) * (this.CurrentResize.YFactor + this.CurrentResize.HeightFactor);
                    grid.Resize(newWidth, newHeight, grid.X - newX, grid.Y - newY);
                    grid.UpdateTiles(this.CurrentResize.StartTiles, this.DragStartX - newX, this.DragStartY - newY);
                    grid.X = newX;
                    grid.Y = newY;
                }
            }
        }

        private void GridCorner_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is Polyline corner && corner.DataContext is Models.Grid grid)
            {
                corner.ReleaseMouseCapture();
                if (grid.X != this.DragStartX || grid.Y != this.DragStartY || grid.Width != this.DragStartWidth || grid.Height != this.DragStartHeight)
                {
                    this.UndoContext.DoAction(new Actions.ResizeGridAction(this.CurrentWorld.Grids.IndexOf(grid), grid.X - this.DragStartX, grid.Y - this.DragStartY, grid.Width - this.DragStartWidth, grid.Height - this.DragStartHeight, this.CurrentResize.StartTiles), false);
                }
            }
            this.CurrentResize = null;
        }
    }
}
