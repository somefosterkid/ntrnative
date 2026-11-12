# A screenshot of bwport.exe's window (its client area: both DS screens as libntr draws them) into a PNG, from the
# screen: the window must be in view (a new window comes up on top; don't cover it) and the desktop session there (an
# RDP session connected, or the console). PrintWindow would read a covered window, but it waits for the window's
# thread, and the game's main thread reads its window messages only once a frame (it hung).
# tools/port_win64.sh run --shots K takes one every K seconds.
#   powershell -ExecutionPolicy Bypass -File port_win64_shot.ps1 -Out C:\bwport\run\shots\10s.png
param([string]$Out, [string]$Process = 'bwport')
Add-Type -AssemblyName System.Drawing
Add-Type @'
using System;
using System.Runtime.InteropServices;
public static class BwShot {
    [StructLayout(LayoutKind.Sequential)] public struct RECT { public int Left, Top, Right, Bottom; }
    [StructLayout(LayoutKind.Sequential)] public struct POINT { public int X, Y; }
    [DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
    [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
    [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
}
'@
[BwShot]::SetProcessDPIAware() | Out-Null
$p = Get-Process $Process -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowHandle -ne 0 } | Select-Object -First 1
if ($null -eq $p) { Write-Output "no $Process window"; exit 1 }
$r = New-Object BwShot+RECT
$o = New-Object BwShot+POINT
[BwShot]::GetClientRect($p.MainWindowHandle, [ref]$r) | Out-Null
[BwShot]::ClientToScreen($p.MainWindowHandle, [ref]$o) | Out-Null
$bmp = New-Object System.Drawing.Bitmap ($r.Right - $r.Left), ($r.Bottom - $r.Top)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($o.X, $o.Y, 0, 0, $bmp.Size)
$bmp.Save($Out, [System.Drawing.Imaging.ImageFormat]::Png)
Write-Output "$Out $($bmp.Width)x$($bmp.Height)"
