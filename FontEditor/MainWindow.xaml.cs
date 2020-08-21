using Microsoft.Win32;
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

namespace FontEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static DependencyProperty CurrentFontProperty = DependencyProperty.Register(nameof(CurrentFont), typeof(SpriteFont), typeof(MainWindow));
        public SpriteFont CurrentFont
        {
            get => (SpriteFont)GetValue(CurrentFontProperty);
            set => SetValue(CurrentFontProperty, value);
        }


        public static DependencyProperty CurrentGlyphProperty = DependencyProperty.Register(nameof(CurrentGlyph), typeof(SpriteFontGlyph), typeof(MainWindow));
        public SpriteFontGlyph CurrentGlyph
        {
            get => (SpriteFontGlyph)GetValue(CurrentGlyphProperty);
            set => SetValue(CurrentGlyphProperty, value);
        }

        public MainWindow()
        {
            InitializeComponent();
            CurrentFont = new SpriteFont();
            foreach (char c in "abcdefghijklmnopqrstuvwxyz")
            {
                CurrentFont.Glyphs.Add(new SpriteFontGlyph(c));
            }
        }

        private void AddGlyphMenuItemClick(object sender, RoutedEventArgs e)
        {
            CurrentFont.Glyphs.Add(new SpriteFontGlyph('A'));
        }

        private void RemoveGlyphMenuItemClick(object sender, RoutedEventArgs e)
        {
            if (GlyphListBox.SelectedIndex != -1)
                CurrentFont.Glyphs.RemoveAt(GlyphListBox.SelectedIndex);
        }

        private void GlyphListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (GlyphListBox.SelectedIndex != -1)
            {
                CurrentGlyph = CurrentFont.Glyphs[GlyphListBox.SelectedIndex];
            }
        }

        private void OpenMenuItemClick(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Polar Vortex Font (*.pvf)|*.pvf";
            if (dialog.ShowDialog() ?? false == true)
            {
                CurrentFont = SpriteFont.LoadFromFile(dialog.FileName);
            }
        }
    }
}
