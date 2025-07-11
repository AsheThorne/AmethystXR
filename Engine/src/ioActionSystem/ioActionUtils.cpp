// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "ioActionUtils.hpp"

AxrBoolInputActionEnum axrWParamToBoolInputActionEnum(const WPARAM wParam) {
    switch (wParam) {
        case VK_LBUTTON: {
            return AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_L;
        }
        case VK_RBUTTON: {
            return AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_R;
        }
        case VK_MBUTTON: {
            return AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_M;
        }
        case VK_XBUTTON1: {
            return AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X1;
        }
        case VK_XBUTTON2: {
            return AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X2;
        }
        case VK_BACK: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_BACKSPACE;
        }
        case VK_TAB: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_TAB;
        }
        case VK_RETURN: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_ENTER;
        }
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_SHIFT;
        }

        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_CTRL;
        }
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_ALT;
        }
        case VK_PAUSE: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_PAUSE;
        }
        case VK_CAPITAL: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_CAPSLOCK;
        }
        case VK_ESCAPE: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_ESCAPE;
        }
        case VK_SPACE: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_SPACE;
        }
        case VK_PRIOR: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_PAGE_UP;
        }
        case VK_NEXT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_PAGE_DOWN;
        }
        case VK_END: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_END_KEY;
        }
        case VK_HOME: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_HOME_KEY;
        }
        case VK_LEFT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_LEFT_ARROW;
        }
        case VK_UP: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_UP_ARROW;
        }
        case VK_RIGHT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_RIGHT_ARROW;
        }
        case VK_DOWN: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_DOWN_ARROW;
        }
        case VK_SNAPSHOT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_PRINT_SCREEN;
        }
        case VK_INSERT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_INSERT;
        }
        case VK_DELETE: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_DELETE;
        }
        case '0': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_0;
        }
        case '1': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_1;
        }
        case '2': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_2;
        }
        case '3': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_3;
        }
        case '4': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_4;
        }
        case '5': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_5;
        }
        case '6': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_6;
        }
        case '7': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_7;
        }
        case '8': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_8;
        }
        case '9': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_9;
        }
        case 'A': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_A;
        }
        case 'B': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_B;
        }
        case 'C': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_C;
        }
        case 'D': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_D;
        }
        case 'E': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_E;
        }
        case 'F': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F;
        }
        case 'G': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_G;
        }
        case 'H': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_H;
        }
        case 'I': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_I;
        }
        case 'J': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_J;
        }
        case 'K': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_K;
        }
        case 'L': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_L;
        }
        case 'M': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_M;
        }
        case 'N': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_N;
        }
        case 'O': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_O;
        }
        case 'P': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_P;
        }
        case 'Q': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_Q;
        }
        case 'R': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_R;
        }
        case 'S': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_S;
        }
        case 'T': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_T;
        }
        case 'U': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_U;
        }
        case 'V': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_V;
        }
        case 'W': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_W;
        }
        case 'X': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_X;
        }
        case 'Y': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_Y;
        }
        case 'Z': {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_Z;
        }
        case VK_LWIN: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_WIN_L;
        }
        case VK_RWIN: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_WIN_R;
        }
        case VK_NUMPAD0: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_0;
        }
        case VK_NUMPAD1: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_1;
        }
        case VK_NUMPAD2: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_2;
        }
        case VK_NUMPAD3: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_3;
        }
        case VK_NUMPAD4: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_4;
        }
        case VK_NUMPAD5: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_5;
        }
        case VK_NUMPAD6: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_6;
        }
        case VK_NUMPAD7: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_7;
        }
        case VK_NUMPAD8: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_8;
        }
        case VK_NUMPAD9: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_9;
        }
        case VK_F1: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F1;
        }
        case VK_F2: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F2;
        }
        case VK_F3: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F3;
        }
        case VK_F4: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F4;
        }
        case VK_F5: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F5;
        }
        case VK_F6: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F6;
        }
        case VK_F7: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F7;
        }
        case VK_F8: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F8;
        }
        case VK_F9: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F9;
        }
        case VK_F10: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F10;
        }
        case VK_F11: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F11;
        }
        case VK_F12: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_F12;
        }
        case VK_NUMLOCK: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_NUM_LOCK;
        }
        case VK_SCROLL: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_SCROLL_LOCK;
        }
        case VK_SUBTRACT: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_SUBTRACT;
        }
        case VK_DECIMAL: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_DECIMAL;
        }
        case VK_DIVIDE: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_DIVIDE;
        }
        case VK_OEM_PLUS: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_PLUS;
        }
        case VK_OEM_COMMA: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_COMMA;
        }
        case VK_OEM_MINUS: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_MINUS;
        }
        case VK_OEM_PERIOD: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_PERIOD;
        }
        case VK_OEM_1: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM1_SEMICOLON_COLON;
        }
        case VK_OEM_2: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM2_FORWARD_SLASH_QUESTION;
        }
        case VK_OEM_3: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM3_BACKTICK_TILDE;
        }
        case VK_OEM_4: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM4_OPEN_SQUARE_BRACKET_BRACES;
        }
        case VK_OEM_5: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM5_BACKSLASH_PIPE;
        }
        case VK_OEM_6: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM6_CLOSED_SQUARE_BRACKET_BRACES;
        }
        case VK_OEM_7: {
            return AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM7_QUOTES;
        }
        default: {
            return AXR_BOOL_INPUT_ACTION_UNDEFINED;
        }
    }
}
