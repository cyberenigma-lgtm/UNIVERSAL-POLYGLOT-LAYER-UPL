// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Diagnostics;
using System.Net;
using System.Net.WebSockets;
using System.Threading;
using System.Threading.Tasks;
using Xunit;

namespace System.Net.Tests
{
    public class HttpListenerWebSocketContextLeakTest
    {
        [Fact]
        [PlatformSpecific(TestPlatforms.Linux)]
        public async Task WebSocketUpgrade_DetachesHttpListenerContext_AndReclaimsMemory()
        {
            using var listener = new HttpListener();
            int port = 59871;
            string prefix = $"http://localhost:{port}/ws/";
            listener.Prefixes.Add(prefix);

            try
            {
                listener.Start();
            }
            catch (HttpListenerException)
            {
                // Port unavailable in test environment, skip gracefully
                return;
            }

            const int Cycles = 100;
            var weakContexts = new WeakReference[Cycles];

            Task serverTask = Task.Run(async () =>
            {
                for (int i = 0; i < Cycles; i++)
                {
                    HttpListenerContext context = await listener.GetContextAsync();
                    weakContexts[i] = new WeakReference(context);

                    HttpListenerWebSocketContext wsContext = await context.AcceptWebSocketAsync(null);
                    using WebSocket ws = wsContext.WebSocket;

                    byte[] buffer = new byte[32];
                    WebSocketReceiveResult result = await ws.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);
                    await ws.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
                }
            });

            for (int i = 0; i < Cycles; i++)
            {
                using var client = new ClientWebSocket();
                await client.ConnectAsync(new Uri($"ws://localhost:{port}/ws/"), CancellationToken.None);
                await client.CloseOutputAsync(WebSocketCloseStatus.NormalClosure, "Done", CancellationToken.None);
            }

            await serverTask;

            // Force full GC collection
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            // Verify that HttpListenerContext instances were cleanly detached and collected
            int aliveCount = 0;
            for (int i = 0; i < Cycles; i++)
            {
                if (weakContexts[i].IsAlive)
                {
                    aliveCount++;
                }
            }

            // In unpatched Unix implementation, all contexts remain anchored by HttpConnection._context
            Assert.True(aliveCount < (Cycles / 2),
                $"Memory leak detected: {aliveCount} of {Cycles} HttpListenerContext instances retained after WebSocket close.");
        }
    }
}
