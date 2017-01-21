#include <criterion/criterion.h>

#include "lazy_winapi.h"

Test(process, get_exe) {
    const wchar_t binary_name[] = L"ut.exe";
    wchar_t buffer[500] = {0};

    cr_assert_not_null(Process_get_exe_path(Process_self(), buffer, sizeof(buffer)),
                      "failed to retrieve exe path");

    cr_assert_not_null(wcsstr(buffer, binary_name), "Couldn't locate own exe name");
}
