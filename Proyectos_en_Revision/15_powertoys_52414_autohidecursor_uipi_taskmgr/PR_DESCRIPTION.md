# Pull Request: Fix AutoHideCursor cursor disappearing over elevated windows (Task Manager)

**Fixes:** https://github.com/microsoft/PowerToys/issues/52414  
**Area:** Mouse Utilities — AutoHideCursor (`src/modules/MouseUtils/AutoHideCursor`)

### Summary
- `AutoHideCursorWorker` already polls `GetCursorPos()` every 100 ms in `State::OnTimer(...)`, and `GetCursorPos()` works across UIPI integrity boundaries even when `Taskmgr.exe` has foreground focus.
- Previously, `State::OnTimer` exited early when `m_hidden` was `true` and never updated `m_lastPointerInputTime` when `cursorPosition` changed without `WH_MOUSE_LL` delivery.
- Updated `State::OnTimer` in `AutoHideCursorState.h` to:
  1. Restore the cursor (`CursorAction::Show`) if `m_hidden` is `true` and `HasMovedIntentionally(cursorPosition)` is satisfied.
  2. Refresh `m_lastPointerInputTime` when `cursorPosition != m_lastPolledPosition` so active pointer movement over Task Manager does not trigger idle hiding.
