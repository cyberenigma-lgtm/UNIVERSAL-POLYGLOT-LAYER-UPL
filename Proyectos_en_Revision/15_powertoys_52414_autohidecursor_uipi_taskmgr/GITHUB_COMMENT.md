Looked into why the cursor vanishes in Task Manager with AutoHideCursor enabled.

`AutoHideCursorWorker.exe` runs at Medium IL (no `uiAccess="true"` in `AutoHideCursorWorker.manifest`), so when an elevated window like `Taskmgr.exe` has focus, `WH_MOUSE_LL` callbacks are subject to UIPI and don't reliably reach `MouseHookProc`.

The worker already polls `GetCursorPos()` every 100 ms and passes it to `State::OnTimer` (`src/modules/MouseUtils/AutoHideCursor/AutoHideCursorState.h`), and `GetCursorPos()` works across integrity levels — but `OnTimer` currently returns early if `m_hidden` is `true`, and never updates `m_lastPointerInputTime` when `cursorPosition` changes without a hook event. As a result, once hidden (or after `idleDelayMs` elapses while moving over Task Manager), moving the mouse won't bring the cursor back.

Checking `HasMovedIntentionally(cursorPosition)` in `OnTimer` when `m_hidden` is `true` and updating `m_lastPointerInputTime` when `cursorPosition` changes fixes both cases without needing `uiAccess` or elevation.

---

**中文说明：** `AutoHideCursorWorker.exe` 以中等权限运行，当任务管理器（`Taskmgr.exe`，高权限窗口）处于前台时，Windows UIPI 会拦截低级鼠标钩子（`WH_MOUSE_LL`）回调。Worker 每 100 ms 已经在通过 `GetCursorPos()` 轮询鼠标坐标（该 API 可跨权限生效），但 `AutoHideCursorState.h` 中的 `OnTimer` 在光标隐藏时直接返回且未检测坐标变化。在 `OnTimer` 中补充坐标移动检测即可在不提权的情况下恢复鼠标显示。

Patch & unit test (`AutoHideCursorState.h`):
https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/tree/main/Proyectos_en_Revision/15_powertoys_52414_autohidecursor_uipi_taskmgr
