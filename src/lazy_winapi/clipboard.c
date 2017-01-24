/**
 * @file
 *
 * Source code of @ref Clipboard module.
 */

#include "clipboard.h"

bool Clipboard_open() {
    return OpenClipboard(0);
}

bool Clipboard_close() {
    return CloseClipboard();
}

bool Clipboard_empty() {
    return EmptyClipboard();
}

size_t Clipboard_get_size(UINT format) {
    const HANDLE clipboard_data = GetClipboardData(format);

    return clipboard_data ? (size_t)GlobalSize(clipboard_data) : 0;
}

size_t Clipboard_get(UINT format, uint8_t *ptr, size_t size) {
    const HANDLE clipboard_data = GetClipboardData(format);

    if (clipboard_data == NULL) {
        return 0;
    }
    else {
        const uint8_t *clipboard_mem = (const uint8_t*)GlobalLock(clipboard_data);

        if (clipboard_mem == NULL) return 0;

        const size_t clipboard_size = (size_t)GlobalSize(clipboard_data);
        const size_t copy_size = clipboard_size > size ? size : clipboard_size;

        (void)memcpy(ptr, clipboard_mem, copy_size);

        (void)GlobalUnlock(clipboard_data);
        return copy_size;
    }
}

bool Clipboard_set(UINT format, const uint8_t *ptr, size_t size) {
    const UINT alloc_flags = GHND;
    const HGLOBAL alloc_handle = GlobalAlloc(alloc_flags, size);

    if (alloc_handle == NULL) return false;

    uint8_t *alloc_mem = (uint8_t*)GlobalLock(alloc_handle);

    (void)memcpy(alloc_mem, ptr, size);
    (void)GlobalUnlock(alloc_handle);
    (void)Clipboard_empty();

    if (SetClipboardData(format, alloc_handle) == NULL) {
        (void)GlobalFree(alloc_handle);
        return false;
    }

    return true;
}

bool Clipboard_set_string(const char *text) {
    const size_t text_len = strlen(text) + 1; //include newline
    return Clipboard_set(CF_TEXT, (uint8_t*)text, text_len);
}

bool Clipboard_set_wide_string(const wchar_t *text) {
    const size_t text_len = (wcslen(text) + 1) * sizeof(wchar_t); //include newline
    return Clipboard_set(CF_UNICODETEXT, (uint8_t*)text, text_len);
}

bool Clipboard_is_format_avail(UINT format) {
    return IsClipboardFormatAvailable(format) != 0;
}

UINT Clipboard_register_format(const wchar_t *name) {
    return RegisterClipboardFormatW(name);
}

/**
 * @return Name of predefined format.
 */
static inline int format_predefined(UINT format, wchar_t* buffer, size_t size) {
    int result = 0;
#define CAT(_left, _right)   _left##_right
#define FORMATTER(_name) case _name: result = swprintf(buffer, size, CAT(L, #_name)); break
    switch (format) {
        FORMATTER(CF_BITMAP);
        FORMATTER(CF_DIB);
        FORMATTER(CF_DIBV5);
        FORMATTER(CF_DIF);
        FORMATTER(CF_DSPBITMAP);
        FORMATTER(CF_DSPENHMETAFILE);
        FORMATTER(CF_DSPMETAFILEPICT);
        FORMATTER(CF_DSPTEXT);
        FORMATTER(CF_ENHMETAFILE);
        FORMATTER(CF_HDROP);
        FORMATTER(CF_LOCALE);
        FORMATTER(CF_METAFILEPICT);
        FORMATTER(CF_OEMTEXT);
        FORMATTER(CF_OWNERDISPLAY);
        FORMATTER(CF_PALETTE);
        FORMATTER(CF_PENDATA);
        FORMATTER(CF_RIFF);
        FORMATTER(CF_SYLK);
        FORMATTER(CF_TEXT);
        FORMATTER(CF_WAVE);
        FORMATTER(CF_TIFF);
        FORMATTER(CF_UNICODETEXT);
        default:
            if (format >= CF_GDIOBJFIRST && format <= CF_GDIOBJLAST) {
                result = swprintf(buffer, size, L"CF_GDIOBJ%d", format - CF_GDIOBJFIRST);
            } else if (format >= CF_PRIVATEFIRST && format <= CF_PRIVATELAST) {
                result = swprintf(buffer, size, L"CF_PRIVATE%d", format - CF_PRIVATEFIRST);
            }
            else {
                return 0;
            }
    }
#undef FORMATTER
#undef CAT
    /**
     * In truncatation case null isn't appended on Win.
     * So append by ourself and do not include null char to result.
     */
    if (result == -1) {
        result = size - 1;
        buffer[result] = 0;
    }
    return result;
}

/**
 * @return Name of user's registered format
 */
static inline int format_custom(UINT format, wchar_t* buffer, size_t size) {
    return GetClipboardFormatNameW(format, buffer, size);
}

int Clipboard_get_format_name(UINT format, wchar_t* buffer, size_t size) {
    return format > 0xC000 ? format_custom(format, buffer, size) : format_predefined(format, buffer, size);
}
