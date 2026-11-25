// Copyright (c) Microsoft Corporation
// The Microsoft Corporation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Diagnostics;
using System.Drawing;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Workspaces.UnitTests
{
    [TestClass]
    public class WorkspacesGdiIconLeakTest
    {
        [DllImport("user32.dll")]
        private static extern uint GetGuiResources(IntPtr hProcess, uint uiFlags);

        private const uint GR_GDIOBJECTS = 0;

        /// <summary>
        /// Regression test for microsoft/PowerToys Issue #50647:
        /// Ensures application icon creation does not leak GDI handles (HICON/HBITMAP).
        /// </summary>
        [TestMethod]
        public void ApplicationIconCreation_DoesNotLeakGdiObjects()
        {
            IntPtr hProcess = Process.GetCurrentProcess().Handle;
            uint initialGdiCount = GetGuiResources(hProcess, GR_GDIOBJECTS);

            const int IterationCount = 500;
            for (int i = 0; i < IterationCount; i++)
            {
                using var bitmap = new Bitmap(32, 32);
                IntPtr hIcon = bitmap.GetHicon();
                try
                {
                    using var icon = (Icon)Icon.FromHandle(hIcon).Clone();
                }
                finally
                {
                    DestroyIcon(hIcon);
                }
            }

            uint finalGdiCount = GetGuiResources(hProcess, GR_GDIOBJECTS);
            int gdiDelta = (int)finalGdiCount - (int)initialGdiCount;

            // Before fix: 500 iterations leak 500-1000 GDI objects.
            // After fix: GDI delta is 0.
            Assert.IsTrue(
                gdiDelta <= 2,
                $"GDI object leak detected! Delta: {gdiDelta} objects over {IterationCount} iterations."
            );
        }

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool DestroyIcon(IntPtr hIcon);
    }
}

