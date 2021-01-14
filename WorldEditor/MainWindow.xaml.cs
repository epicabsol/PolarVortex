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
                if (WorldElement != null)
                {
                    WorldElement.LayoutTransform = new ScaleTransform(Scale, Scale);
                }
            }
        }

        public MainWindow(string baseDirectory, Models.World world)
        {
            this.BaseDirectory = baseDirectory;
            this.CurrentWorld = world;

            InitializeComponent();
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
    }
}
