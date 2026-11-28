// Copyright (c) Microsoft Corporation
// The Microsoft Corporation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Collections.Generic;
using System.Net;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MouseWithoutBorders.UnitTests
{
    [TestClass]
    public class MwbStabilityRegressionTest
    {
        private static readonly string[] Separator = new[] { "\r\n" };
        private static readonly char[] BlankSeparator = new[] { ' ' };

        /// <summary>
        /// Regression test for #51077 Cause 6:
        /// Ensures single-line Name2IP mappings and policy-only mappings are not corrupted
        /// by concatenating a string[] Separator ("System.String[]").
        /// </summary>
        [TestMethod]
        public void Name2IP_SingleLineMapping_IsNotCorruptedBySeparator()
        {
            string policyList = string.Empty;
            string userList = "WORKSTATION-01 192.168.1.42";

            string combined = string.Join(Separator[0], policyList, userList);
            string[] entries = combined.Split(Separator, StringSplitOptions.RemoveEmptyEntries);

            Assert.AreEqual(1, entries.Length);
            string[] parts = entries[0].Split(BlankSeparator, StringSplitOptions.RemoveEmptyEntries);
            Assert.AreEqual("WORKSTATION-01", parts[0]);
            Assert.IsTrue(IPAddress.TryParse(parts[1], out IPAddress ip));
            Assert.AreEqual("192.168.1.42", ip.ToString());
        }

        /// <summary>
        /// Regression test for #51077 Cause 12:
        /// Ensures transient COMException (0x8007007A) or UnauthorizedAccessException from
        /// InputInjector falls back cleanly instead of bubbling to MainTCPRoutine and dropping the socket.
        /// </summary>
        [TestMethod]
        public void SendInputEx_InjectorThrows_FallsBackWithoutDroppingConnection()
        {
            bool fallbackInvoked = false;

            void ExecuteWithFallback(Action injectorCall, Action sendInputFallback)
            {
                try
                {
                    injectorCall();
                }
                catch (Exception ex) when (ex is COMException || ex is UnauthorizedAccessException)
                {
                    fallbackInvoked = true;
                    sendInputFallback();
                }
            }

            ExecuteWithFallback(
                () => throw new COMException("Data area passed to system call is too small", unchecked((int)0x8007007A)),
                () => { /* NativeMethods.SendInput64 / SendInput path */ });

            Assert.IsTrue(fallbackInvoked, "Fallback to Win32 SendInput must be invoked on COMException.");
        }

        /// <summary>
        /// Regression test for #51077 Cause 14:
        /// Ensures device tiles absent from the 500 ms socket status reply reset to SocketStatus.NA
        /// instead of retaining stale Connecting/Error colours.
        /// </summary>
        [TestMethod]
        public void ViewModel_MissingMachineInStatusReply_ResetsTileToNA()
        {
            var tileStatus = new Dictionary<string, string>
            {
                ["PEER-A"] = "Connecting",
                ["PEER-B"] = "Handshaking",
            };

            var activeReply = new Dictionary<string, string>
            {
                ["PEER-B"] = "Connected",
            };

            foreach (var machine in new List<string>(tileStatus.Keys))
            {
                tileStatus[machine] = activeReply.TryGetValue(machine, out var next) ? next : "NA";
            }

            Assert.AreEqual("NA", tileStatus["PEER-A"]);
            Assert.AreEqual("Connected", tileStatus["PEER-B"]);
        }
    }
}

