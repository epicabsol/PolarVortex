using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WorldEditor
{
    /// <summary>
    /// Interaction logic for StartWindow.xaml
    /// </summary>
    public partial class StartWindow : Window
    {
        public string BaseDirectory = @"E:\Projects\PolarVortex";

        public StartWindow()
        {
            InitializeComponent();
        }

        private void NewWorldButton_Click(object sender, RoutedEventArgs e)
        {
            /*MainWindow window = new MainWindow();
            window.Show();*/
            CreateWorldWindow window = new CreateWorldWindow(BaseDirectory);
            window.Show();
            Close();
        }

        private void OpenWorldButton_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dialog = new Microsoft.Win32.OpenFileDialog();
            dialog.Filter = Models.World.DialogFilter;
            dialog.InitialDirectory = BaseDirectory;
            if (dialog.ShowDialog() ?? false)
            {
                Models.World world = Models.World.LoadFromFile(dialog.FileName);
                MainWindow window = new MainWindow(BaseDirectory, world, dialog.FileName);
                window.Show();
                Close();
            }
        }
    }
}
