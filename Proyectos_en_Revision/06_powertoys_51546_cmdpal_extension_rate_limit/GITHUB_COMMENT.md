@Chaos02 @jiripolasek @zadjii-msft

Adding to @Chaos02's observation: while #51575 effectively resolved the unbounded collection growth (`LogMessages` UI view model) that was causing multi-GB RAM bloat in `Microsoft.CmdPal.UI.exe`, the IPC layer remains vulnerable to chatty or looping extensions.

When an out-of-process extension enters a tight logging loop, every call still flows through `IExtensionHost -> NdrExtInterfacePointerUnmarshall -> CoUnmarshalInterface`, which explains the ~49% CPU profile spike and aggressive disk write thrashing.

### Proposed Solution: O(1) Token Bucket in `AppExtensionHost.LogMessage`
We can protect both COM IPC throughput and disk I/O by introducing a lightweight, lock-free Token Bucket per extension in `AppExtensionHost.cs`:

- **Capacity & Refill:** 50 messages/second per extension ID.
- **Lock-Free / O(1):** Atomic `Interlocked.Decrement` on available tokens; zero lock contention.
- **Burst Aggregation:** Suppressed calls increment an atomic counter. When the window elapses, a single aggregated entry is logged:
  `[AppExtensionHost] Throttled {count} burst log messages from extension '{id}' to preserve IPC throughput.`

```csharp
// AppExtensionHost.cs
private static readonly ConcurrentDictionary<string, ExtensionLogRateLimiter> _rateLimiters = new();

public void LogMessage(ILogMessage message)
{
    if (message == null) return;

    string extensionId = _extension?.ExtensionId ?? "unknown";
    var limiter = _rateLimiters.GetOrAdd(extensionId, _ => new ExtensionLogRateLimiter());

    if (!limiter.TryConsume(out int flushedThrottled))
    {
        return; // Suppress burst before disk/IPC saturation
    }

    if (flushedThrottled > 0)
    {
        Logger.LogWarning($"[AppExtensionHost] Throttled {flushedThrottled} burst log messages from extension '{extensionId}'.");
    }

    Logger.Log(message.Message);
}
```

This ensures chatty or misbehaving extensions can never starve the Command Palette UI thread or saturate disk I/O, providing defense-in-depth on top of #51575.

Standalone audit and unit test available here:
https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/tree/main/Proyectos_en_Revision/06_powertoys_51546_cmdpal_extension_rate_limit
