#include <criterion/criterion.h>

#include "lazy_winapi.h"

struct error_desc_test {
    DWORD error;
    wchar_t *expect_desc;
};

/**
 * Test error description extraction.
 */
Test(error, error_description_known) {
    wchar_t buffer[512] = {0};
    const size_t len = sizeof(buffer) / sizeof(buffer[0]);

    static struct error_desc_test params[] = {
        {
            0,
            L"The operation completed successfully."
        },
        {
            1,
            L"Incorrect function."
        },
        {
            666,
            L"Unknown Error."
        }
    };

    const size_t params_len = sizeof(params) / sizeof(params[0]);
    for (size_t idx = 0; idx < params_len; idx++) {
        Error_get_desc(params[idx].error, buffer, len);

        cr_expect_wcs_eq(buffer, params[idx].expect_desc);
    }
}

/**
 * Test on buffer size 1.
 * Since WinAPI adds newline to size the output is empty.
 */
Test(error, error_description_trunc_empty) {
    wchar_t buffer[1] = {0};
    const size_t len = sizeof(buffer) / sizeof(buffer[0]);
    const wchar_t expect_desc[] = L"";

    cr_assert_not_null(Error_get_desc(0, buffer, len));

    cr_assert_wcs_eq(buffer, expect_desc);
}

/**
 * Test error description truncation.
 */
Test(error, error_description_trunc) {
    wchar_t buffer[20] = {0};
    const size_t len = sizeof(buffer) / sizeof(buffer[0]);
    const wchar_t expect_desc[] = L"The operation compl";

    cr_assert_not_null(Error_get_desc(0, buffer, len));

    cr_assert_wcs_eq(buffer, expect_desc);
}

/**
 * Test error description invalid usage.
 */
Test(error, error_description_invalid) {
    wchar_t buffer[512] = {0};
    const size_t len = sizeof(buffer) / sizeof(buffer[0]);

    cr_assert_null(Error_get_desc(1, 0, 0));
    cr_assert_null(Error_get_desc(1, buffer, 0));
    cr_assert_null(Error_get_desc(1, 0, len));
}
