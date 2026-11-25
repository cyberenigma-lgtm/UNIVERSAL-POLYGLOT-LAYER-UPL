## Description
Fixes #50647

In Workspaces, application icon extraction leaks GDI handles and bitmaps:
1. `Bitmap.GetHicon()` returns an unmanaged `HICON` handle. `Icon.FromHandle(iconHandle)` does not take ownership, leaking one GDI icon handle per application icon.
2. The source `Bitmap` instances were not disposed.
3. `DrawHelper.SaveBitmap` left unmanaged `EncoderParameters` undisposed.
4. `BaseApplication.Dispose()` did not dispose `_icon`.

This PR:
1. Clones the icon and calls `DestroyIcon` on the native handle in a `finally` block (matching Peek's `BitmapHelper` pattern).
2. Disposes source bitmaps and `EncoderParameters`.
3. Calls `_icon?.Dispose()` in `BaseApplication.Dispose()`.
4. Adds regression unit test verifying GDI object count stability.
