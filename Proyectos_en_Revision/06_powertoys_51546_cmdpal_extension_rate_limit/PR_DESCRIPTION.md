## Summary
Prevents IPC saturation, COM unmarshalling CPU spikes (49%), and disk I/O thrashing caused by chatty or looping out-of-process extensions in Command Palette.

Relates to #51546.

### Proposed Changes
- Introduce lock-free, per-extension `ExtensionLogRateLimiter` (Token Bucket with 50 tokens/sec capacity) in `AppExtensionHost.cs`.
- Suppress runaway burst calls in $O(1)$ time and aggregate dropped messages into periodic summary warnings.
- Add unit tests verifying rate limiter behavior under single-threaded and multi-threaded bursts.
