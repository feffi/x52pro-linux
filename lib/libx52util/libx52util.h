/*
 * Saitek X52 Pro Utility Library
 *
 * Copyright (C) 2015 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#ifndef LIBX52UTIL_H
#define LIBX52UTIL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert UTF8 string to X52 character map.
 *
 * This function takes in a UTF-8 string and converts it to the character
 * map used by the X52Pro MFD. Unrecognized characters are silently dropped.
 *
 * @param[in]       input   Input string in UTF-8. Must be NUL-terminated
 * @param[out]      output  Output buffer
 * @param[inout]    len     Length of output buffer
 *
 * @returns 0 on success, -EINVAL on invalid parameters, -E2BIG if the buffer
 * filled up before converting the entire string.
 */
int libx52util_convert_utf8_string(const uint8_t *input,
                                   uint8_t *output, size_t *len);

#ifdef __cplusplus
}
#endif

#endif /* !defined LIBX52UTIL_H */

