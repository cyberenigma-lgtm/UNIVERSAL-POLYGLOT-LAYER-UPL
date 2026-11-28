# Issue #52414: AutoHideCursor Cursor Disappears Over Task Manager (UIPI)

**Target Repository:** [microsoft/PowerToys](https://github.com/microsoft/PowerToys)  
**Target Issue:** [#52414](https://github.com/microsoft/PowerToys/issues/52414)  
**Component:** `src/modules/MouseUtils/AutoHideCursor`

---

## Root Cause

`AutoHideCursorWorker.exe` runs at Medium integrity without `uiAccess="true"`. When an elevated window such as Task Manager (`Taskmgr.exe`) has focus, Windows UIPI prevents `WH_MOUSE_LL` from updating `State` consistently, while `State::OnTimer` (every 100 ms using `GetCursorPos()`, which works across integrity levels) returns `CursorAction::None` immediately when `m_hidden` is `true` and never updates `m_lastPointerInputTime` when `cursorPosition` changes.

## Fix

In `AutoHideCursorState.h`:
1. When `m_hidden` is `true`, `OnTimer` checks `HasMovedIntentionally(cursorPosition)` and restores the cursor (`CursorAction::Show`).
2. When `m_hidden` is `false`, `OnTimer` compares `cursorPosition` against `m_lastPolledPosition` and refreshes `m_lastPointerInputTime = now` on movement so the idle timer does not hide the cursor while the user is actively moving it over Task Manager.

## Verification

```bash
uplc run autohidecursor_uipi_audit.upl
```
