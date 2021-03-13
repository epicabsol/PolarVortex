using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Interop;

namespace WorldEditor
{
    [System.Windows.TemplatePart(Name = nameof(PART_MinimizeButton), Type = typeof(Button))]
    [System.Windows.TemplatePart(Name = nameof(PART_MaximizeButton), Type = typeof(Button))]
    [System.Windows.TemplatePart(Name = nameof(PART_RestoreButton), Type = typeof(Button))]
    [System.Windows.TemplatePart(Name = nameof(PART_CloseButton), Type = typeof(Button))]
    public class Window : System.Windows.Window
    {
        private Button PART_MinimizeButton;
        private Button PART_MaximizeButton;
        private Button PART_RestoreButton;
        private Button PART_CloseButton;

        public Window()
        {
            this.DefaultStyleKey = typeof(Window);
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            // HACK: Overriding WM_GETMINMAXINFO doesn't work for windows where the ResizeMode is resizable. WPF must be overriding it or something.
            // So, we'll leave this code here and in the meantime, hack around the issue by showing a 7px border in maximized mode using the ControlTemplate.
            //((HwndSource)System.Windows.PresentationSource.FromVisual(this)).AddHook(this.WndProcHook);
            base.OnSourceInitialized(e);
        }

        public override void OnApplyTemplate()
        {
            this.PART_MinimizeButton = this.GetTemplateChild(nameof(PART_MinimizeButton)) as Button;
            this.PART_MinimizeButton.Click += (sender, args) => this.WindowState = System.Windows.WindowState.Minimized;
            this.PART_MaximizeButton = this.GetTemplateChild(nameof(PART_MaximizeButton)) as Button;
            this.PART_MaximizeButton.Click += (sender, args) => this.WindowState = System.Windows.WindowState.Maximized;
            this.PART_RestoreButton = this.GetTemplateChild(nameof(PART_RestoreButton)) as Button;
            this.PART_RestoreButton.Click += (sender, args) => this.WindowState = System.Windows.WindowState.Normal;
            this.PART_CloseButton = this.GetTemplateChild(nameof(PART_CloseButton)) as Button;
            this.PART_CloseButton.Click += (sender, args) => this.Close();

            base.OnApplyTemplate();
        }

        private const int WM_GETMINMAXINFO = 0x0024;
        private const uint MONITOR_DEFAULTTONEAREST = 0x00000002;

        [DllImport("user32.dll")]
        private static extern IntPtr MonitorFromWindow(IntPtr handle, uint flags);

        [DllImport("user32.dll")]
        private static extern bool GetMonitorInfo(IntPtr hMonitor, ref MONITORINFO lpmi);

        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;

            public RECT(int left, int top, int right, int bottom)
            {
                this.Left = left;
                this.Top = top;
                this.Right = right;
                this.Bottom = bottom;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct MONITORINFO
        {
            public int cbSize;
            public RECT rcMonitor;
            public RECT rcWork;
            public uint dwFlags;
        }

        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public int X;
            public int Y;

            public POINT(int x, int y)
            {
                this.X = x;
                this.Y = y;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct MINMAXINFO
        {
            public POINT ptReserved;
            public POINT ptMaxSize;
            public POINT ptMaxPosition;
            public POINT ptMinTrackSize;
            public POINT ptMaxTrackSize;
        }

        private IntPtr WndProcHook(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == WM_GETMINMAXINFO)
            {
                MINMAXINFO mmi = (MINMAXINFO)Marshal.PtrToStructure(lParam, typeof(MINMAXINFO));
                IntPtr monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
                if (monitor != IntPtr.Zero)
                {
                    MONITORINFO monitorInfo = new MONITORINFO();
                    monitorInfo.cbSize = Marshal.SizeOf(typeof(MONITORINFO));
                    GetMonitorInfo(monitor, ref monitorInfo);
                    RECT workArea = monitorInfo.rcWork;
                    RECT monitorArea = monitorInfo.rcMonitor;
                    mmi.ptMaxPosition.X = workArea.Left - monitorArea.Left - 1;
                    mmi.ptMaxPosition.Y = workArea.Top - monitorArea.Top - 1;
                    mmi.ptMaxSize.X = workArea.Right - workArea.Left + 2;
                    mmi.ptMaxSize.Y = workArea.Bottom - workArea.Top + 2;
                    mmi.ptMaxTrackSize = mmi.ptMaxSize;
                    handled = true;
                }

                Marshal.StructureToPtr(mmi, lParam, true);
            }
            return IntPtr.Zero;
        }
    }
}
