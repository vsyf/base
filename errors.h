/*
 * errors.h
 * Copyright (C) 2021 youfa.song <vsyfar@gmail.com>
 *
 * Distributed under terms of the GPLv2 license.
 */

#ifndef AVE_ERROR_H
#define AVE_ERROR_H

#include <cerrno>
#include <cstdint>
#include <string>

namespace ave {

using status_t = int32_t;

enum int32_t {
  OK = 0,         // Preferred constant for checking success.
  NO_ERROR = OK,  // Deprecated synonym for `OK`. Prefer `OK` because it doesn't
                  // conflict with Windows.

  UNKNOWN_ERROR = (-2147483647 - 1),  // INT32_MIN value

  E_AGAIN = -EAGAIN,
  NO_MEMORY = -ENOMEM,
  INVALID_OPERATION = -ENOSYS,
  BAD_VALUE = -EINVAL,
  BAD_TYPE = (UNKNOWN_ERROR + 1),
  NAME_NOT_FOUND = -ENOENT,
  PERMISSION_DENIED = -EPERM,
  NO_INIT = -ENODEV,
  ALREADY_EXISTS = -EEXIST,
  DEAD_OBJECT = -EPIPE,
  FAILED_TRANSACTION = (UNKNOWN_ERROR + 2),
#if !defined(_WIN32)
  BAD_INDEX = -EOVERFLOW,
  NOT_ENOUGH_DATA = -ENODATA,
  WOULD_BLOCK = -EWOULDBLOCK,
  TIMED_OUT = -ETIMEDOUT,
  UNKNOWN_TRANSACTION = -EBADMSG,
#else
  BAD_INDEX = -E2BIG,
  NOT_ENOUGH_DATA = (UNKNOWN_ERROR + 3),
  WOULD_BLOCK = (UNKNOWN_ERROR + 4),
  TIMED_OUT = (UNKNOWN_ERROR + 5),
  UNKNOWN_TRANSACTION = (UNKNOWN_ERROR + 6),
#endif
  FDS_NOT_ALLOWED = (UNKNOWN_ERROR + 7),
  UNEXPECTED_NULL = (UNKNOWN_ERROR + 8),
};

std::string StatusToString(status_t status);

} /* namespace ave */

#endif /* !AVE_ERROR_H */
