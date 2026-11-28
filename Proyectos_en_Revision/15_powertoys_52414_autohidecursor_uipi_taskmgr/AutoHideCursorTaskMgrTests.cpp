#include "pch.h"
#include "CppUnitTest.h"
#include "../AutoHideCursorState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AutoHideCursorUnitTests
{
    TEST_CLASS(AutoHideCursorTaskMgrTests)
    {
    public:
        TEST_METHOD(OnTimer_ShowsHiddenCursor_WhenCursorPosMovesWithoutMouseHook)
        {
            auto_hide_cursor::Configuration config{ true, true, 5000 };
            auto_hide_cursor::State state{ config, 1000, { 500, 500 } };

            Assert::AreEqual(
                static_cast<int>(auto_hide_cursor::CursorAction::Hide),
                static_cast<int>(state.OnKeyboardInput(1000, { 500, 500 })));

            // Simulate UIPI blocking WH_MOUSE_LL over Task Manager while GetCursorPos moves
            Assert::AreEqual(
                static_cast<int>(auto_hide_cursor::CursorAction::Show),
                static_cast<int>(state.OnTimer(1100, { 520, 510 })));
            Assert::IsFalse(state.IsHidden());
        }

        TEST_METHOD(OnTimer_DoesNotHideCursor_WhenCursorPosMovesOverElevatedWindow)
        {
            auto_hide_cursor::Configuration config{ true, true, 5000 };
            auto_hide_cursor::State state{ config, 0, { 100, 100 } };

            Assert::AreEqual(
                static_cast<int>(auto_hide_cursor::CursorAction::None),
                static_cast<int>(state.OnTimer(5100, { 300, 300 })));
            Assert::IsFalse(state.IsHidden());
        }
    };
}
