#include <mimick.h>
#include <mimick/assert.h>

#include <criterion/criterion.h>

#include "lazy_winapi.h"

mmk_mock_define(GetClipboardData_mock, HANDLE WINAPI, UINT);
mmk_mock_define(GlobalLock_mock, LPVOID, HGLOBAL);
mmk_mock_define(GlobalAlloc_mock, HGLOBAL, UINT, SIZE_T);
mmk_mock_define(SetClipboardData_mock, HANDLE, UINT, HANDLE);

#define CREATE_MOCK(name) cr_assert( (mmk_fn) mmk_mock(#name "@self", name ## _mock) != MMK_MOCK_INVALID, #name " mock creation failed")
#define MOCK_RETURN_PTR(_data, _type) &(_type){ &_data }

static const DWORD format = CF_TEXT;

static void setup() {
    const char text[] = "For my waifu!";

    cr_assert(Clipboard_open(), "Cannot open clipboard");
    cr_assert(Clipboard_set_string(text), "Cannot set clipboard text");
}

static void teardown() {
    cr_assert(Clipboard_close(), "Cannot close clipboard");
}

TestSuite(clipboard_mock, .init = setup, .fini = teardown);

/**
 * Test try to get clipboard text
 *
 * GetClipboardData mock returns NULL.
 */
Test(clipboard_mock, get_fail_no_data) {
    char extract_text[50] = {0};

    CREATE_MOCK(GetClipboardData);
    mmk_when(GetClipboardData(format), .then_return = NULL);

    cr_assert_eq(Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)), 0);

    cr_assert(mmk_verify(GetClipboardData(format), .times = 1),
              "GetClipboardData incorrect invokation");

    mmk_reset(GetClipboardData);
}

/**
 * Test try to get clipboard text
 *
 * GetClipboardData mock returns dummy.
 * GlobalLock mock returns NULL
 */
Test(clipboard_mock, get_fail_no_lock) {
    char extract_text[50] = {0};

    int mock_data = 1;

    CREATE_MOCK(GetClipboardData);
    mmk_when(GetClipboardData(format), .then_return = MOCK_RETURN_PTR(mock_data, HANDLE) );

    CREATE_MOCK(GlobalLock);
    mmk_when(GlobalLock(mmk_eq(HGLOBAL, &mock_data)), .then_return = NULL);

    cr_assert_eq(Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)), 0);

    cr_assert(mmk_verify(GetClipboardData(mmk_eq(UINT, format)), .times = 1),
              "GetClipboardData incorrect invokation");
    cr_assert(mmk_verify(GlobalLock(mmk_eq(HGLOBAL, &mock_data)), .times = 1),
              "GlobalLock incorrect invokation");

    mmk_reset(GetClipboardData);
    mmk_reset(GlobalLock);
}

/**
 * Test try to set text.
 *
 * GlobalAlloc mock returns NULL
 */
Test(clipboard_mock, set_fail_no_memory) {
    const char text[] = "Mock!";
    const size_t len = strlen(text) + 1;

    CREATE_MOCK(GlobalAlloc);
    mmk_when(GlobalAlloc(mmk_any(UINT), mmk_eq(SIZE_T, len)), .then_return = NULL);

    cr_assert_eq(Clipboard_set_string(text), 0, "Should fail to set clipboard!");
    cr_assert(mmk_verify(GlobalAlloc(mmk_any(UINT), mmk_eq(UINT, len)), .times = 1),
              "GlobalAlloc incorrect invokation");

    mmk_reset(GlobalAlloc);
}

/**
 * Test try to set text.
 *
 * SetClipboardData mock returns NULL
 */
Test(clipboard_mock, set_just_fail) {
    const char text[] = "Mock!";

    CREATE_MOCK(SetClipboardData);
    mmk_when(SetClipboardData(mmk_any(UINT), mmk_any(HANDLE)), .then_return = NULL);

    cr_assert_eq(Clipboard_set_string(text), 0, "Should fail to set clipboard!");
    cr_assert(mmk_verify(SetClipboardData(mmk_any(UINT), mmk_any(HANDLE)), .times = 1),
              "SetClipboardData incorrect invokation");

    mmk_reset(SetClipboardData);
}
