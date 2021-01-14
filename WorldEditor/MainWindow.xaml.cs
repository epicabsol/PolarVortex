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

        public MainWindow(string baseDirectory, Models.World world)
        {
            this.BaseDirectory = baseDirectory;
            this.CurrentWorld = world;

            InitializeComponent();
        }

        private void ZoomInButton_Click(object sender, RoutedEventArgs e)
        {
            WorldElement.Scale = WorldElement.Scale * 2.0f;
        }

        private void ZoomOutButton_Click(object sender, RoutedEventArgs e)
        {
            WorldElement.Scale = WorldElement.Scale * 0.5f;
        }

        private void WorldElement_ScaleChanged(object sender, float e)
        {
            if (ZoomLabel != null)
            {
                ZoomLabel.Text = $"{e}x";
            }
        }
    }
}
