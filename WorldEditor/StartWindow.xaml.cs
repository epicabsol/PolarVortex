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
        public StartWindow()
        {
            InitializeComponent();
        }

        private void NewWorldButton_Click(object sender, RoutedEventArgs e)
        {
            /*MainWindow window = new MainWindow();
            window.Show();*/
            CreateWorldWindow window = new CreateWorldWindow();
            window.Show();
            Close();
        }
    }
}
