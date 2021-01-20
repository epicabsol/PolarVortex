using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    /// Interaction logic for CreateWorldWindow.xaml
    /// </summary>
    public partial class CreateWorldWindow : Window
    {
        public string BaseDirectory { get; }
        public ObservableCollection<string> PalettePaths { get; }
        public string PalettePreviewPath { get; set; } = "";

        public string PalettePath { get; set; } = "";
        public int WorldWidth { get; set; } = 50;
        public int WorldHeight { get; set; } = 20;

        public CreateWorldWindow(string baseDirectory)
        {
            this.BaseDirectory = baseDirectory;
            this.PalettePaths = new ObservableCollection<string>();
            this.RefreshPalettePaths();
            this.RefreshPalettePreviewPath();
            InitializeComponent();
        }

        private void RefreshPalettePaths()
        {
            this.PalettePaths.Clear();
            foreach (string palettePath in System.IO.Directory.EnumerateFiles(BaseDirectory, "*.pvp", System.IO.SearchOption.AllDirectories))
            {
                if (palettePath.Contains(@"\bin\"))
                    continue;
                this.PalettePaths.Add(palettePath.Substring(BaseDirectory.Length + 1).Replace('\\', '/'));
            }
            if (this.PalettePaths.Count > 0 && (this.PalettePath == null || this.PalettePath == ""))
            {
                this.PalettePath = this.PalettePaths[0];
            }
        }

        private void RefreshPalettePreviewPath()
        {
            if (String.IsNullOrEmpty(this.PalettePath))
            {
                this.PalettePreviewPath = "";
            }
            else
            {
                string path = System.IO.Path.Combine(this.BaseDirectory, this.PalettePath);
                if (System.IO.File.Exists(path))
                {
                    try
                    {
                        dynamic data = Newtonsoft.Json.JsonConvert.DeserializeObject(System.IO.File.ReadAllText(path));
                        this.PalettePreviewPath = System.IO.Path.Combine(this.BaseDirectory, (string)data.texture);
                    }
                    catch (Exception ex)
                    {
                        this.PalettePreviewPath = "";
                    }
                }
                else
                {
                    this.PalettePreviewPath = "";
                }
            }
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindow window = new MainWindow(this.BaseDirectory, new Models.World(this.PalettePath, this.WorldWidth, this.WorldHeight), null);
            window.Show();
            Close();
        }

        private void RefreshButton_Click(object sender, RoutedEventArgs e)
        {
            this.RefreshPalettePaths();
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            this.PalettePath = this.PaletteComboBox.SelectedItem as string;
            RefreshPalettePreviewPath();
        }
    }
}
