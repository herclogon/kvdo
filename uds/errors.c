/*
 * Copyright (c) 2020 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA. 
 *
 * $Id: //eng/uds-releases/jasper/src/uds/errors.c#10 $
 */

#include "errors.h"

#include "common.h"
#include "permassert.h"
#include "stringUtils.h"

#ifdef __KERNEL__
#include <linux/errno.h>
#endif

static const struct errorInfo successful = { "UDS_SUCCESS", "Success" };

#ifdef __KERNEL__
static const char *const messageTable[] = {
  [EPERM]   = "Operation not permitted",
  [ENOENT]  = "No such file or directory",
  [ESRCH]   = "No such process",
  [EINTR]   = "Interrupted system call",
  [EIO]     = "Input/output error",
  [ENXIO]   = "No such device or address",
  [E2BIG]   = "Argument list too long",
  [ENOEXEC] = "Exec format error",
  [EBADF]   = "Bad file descriptor",
  [ECHILD]  = "No child processes",
  [EAGAIN]  = "Resource temporarily unavailable",
  [ENOMEM]  = "Cannot allocate memory",
  [EACCES]  = "Permission denied",
  [EFAULT]  = "Bad address",
  [ENOTBLK] = "Block device required",
  [EBUSY]   = "Device or resource busy",
  [EEXIST]  = "File exists",
  [EXDEV]   = "Invalid cross-device link",
  [ENODEV]  = "No such device",
  [ENOTDIR] = "Not a directory",
  [EISDIR]  = "Is a directory",
  [EINVAL]  = "Invalid argument",
  [ENFILE]  = "Too many open files in system",
  [EMFILE]  = "Too many open files",
  [ENOTTY]  = "Inappropriate ioctl for device",
  [ETXTBSY] = "Text file busy",
  [EFBIG]   = "File too large",
  [ENOSPC]  = "No space left on device",
  [ESPIPE]  = "Illegal seek",
  [EROFS]   = "Read-only file system",
  [EMLINK]  = "Too many links",
  [EPIPE]   = "Broken pipe",
  [EDOM]    = "Numerical argument out of domain",
  [ERANGE]  = "Numerical result out of range"
};
#endif

static const struct errorInfo errorList[] = {
  { "UDS_UNINITIALIZED", "UDS library is not initialized" },
  { "UDS_SHUTTINGDOWN", "UDS library is shutting down" },
  { "UDS_EMODULE_LOAD", "Could not load modules" },
  { "UDS_ENOTHREADS", "Could not create a new thread" },
  { "UDS_NOCONTEXT", "Could not find the requested library context" },
  { "UDS_DISABLED", "UDS library context is disabled" },
  { "UDS_CORRUPT_COMPONENT", "Corrupt saved component" },
  { "UDS_UNKNOWN_ERROR", "Unknown error" },
  { "UDS_UNUSED_CODE_8", "Unused error code 8" },
  { "UDS_UNUSED_CODE_9", "Unused error code 9" },
  { "UDS_UNSUPPORTED_VERSION", "Unsupported version" },
  { "UDS_NO_INDEXSESSION", "Index session not known" },
  { "UDS_CORRUPT_DATA", "Index data in memory is corrupt" },
  { "UDS_SHORT_READ", "Could not read requested number of bytes" },
  { "UDS_UNUSED_CODE_14", "Unused error code 14" },
  { "UDS_RESOURCE_LIMIT_EXCEEDED", "Internal resource limits exceeded" },
  { "UDS_VOLUME_OVERFLOW", "Memory overflow due to storage failure" },
  { "UDS_UNUSED_CODE_17", "Unused error code 17" },
  { "UDS_UNUSED_CODE_18", "Unused error code 18" },
  { "UDS_UNUSED_CODE_19", "Unused error code 19" },
  { "UDS_CONF_PTR_REQUIRED", "A configuration pointer is required" },
  { "UDS_INDEX_STATS_PTR_REQUIRED", "An index stats pointer is required" },
  { "UDS_CONTEXT_STATS_PTR_REQUIRED", "A context stats pointer is required" },
  { "UDS_UNUSED_CODE_23", "Unused error code 23" },
  { "UDS_UNUSED_CODE_24", "Unused error code 24" },
  { "UDS_UNUSED_CODE_25", "Unused error code 25" },
  { "UDS_UNUSED_CODE_26", "Unused error code 26" },
  { "UDS_UNUSED_CODE_27", "Unused error code 27" },
  { "UDS_INVALID_MEMORY_SIZE",
    "Configured memory too small or unsupported size" },
  { "UDS_UNUSED_CODE_29", "Unused error code 29" },
  { "UDS_INDEX_NAME_REQUIRED", "An index name is required" },
  { "UDS_CONF_REQUIRED", "A configuration is required" },
  { "UDS_UNUSED_CODE_32", "Unused error code 32" },
  { "UDS_UNUSED_CODE_33", "Unused error code 33" },
  { "UDS_UNUSED_CODE_34", "Unused error code 34" },
  { "UDS_UNUSED_CODE_35", "Unused error code 35" },
  { "UDS_UNUSED_CODE_36", "Unused error code 36" },
  { "UDS_NO_INDEX", "No index found" },
  { "UDS_BAD_CHECKPOINT_FREQUENCY", "Checkpoint frequency out of range" },
  { "UDS_WRONG_INDEX_CONFIG", "Wrong type of index configuration" },
  { "UDS_UNUSED_CODE_40", "Unused error code 40" },
  { "UDS_UNUSED_CODE_41", "Unused error code 41" },
  { "UDS_UNUSED_CODE_42", "Unused error code 42" },
  { "UDS_UNUSED_CODE_43", "Unused error code 43" },
  { "UDS_END_OF_FILE", "Unexpected end of file" },
  { "UDS_INDEX_NOT_SAVED_CLEANLY", "Index not saved cleanly" },
  { "UDS_UNUSED_CODE_46", "Unused error code 46" },
  { "UDS_INSUFFICIENT_INDEX_SPACE", "Insufficient index space" },
  { "UDS_UNUSED_CODE_48", "Unused error code 48" },
  { "UDS_UNUSED_CODE_49", "Unused error code 49" },
  { "UDS_SUSPENDED", "Index suspended"},
  { "UDS_INVALID_OPERATION", "Invalid session operation"},
  { "UDS_INDEXSESSION_IN_USE", "Index session in use"},
  { "UDS_CALLBACK_REQUIRED", "A callback function is required"},
  { "UDS_INVALID_OPERATION_TYPE", "Invalid type of request operation"},
};

static const struct errorInfo internalErrorList[] = {
  { "UDS_INTERNAL_UNUSED_0", "Unused internal error 0" },
  { "UDS_OVERFLOW", "Index overflow" },
  { "UDS_INTERNAL_UNUSED_2", "Unused internal error 2" },
  { "UDS_INVALID_ARGUMENT", "Invalid argument passed to internal routine" },
  { "UDS_BAD_STATE", "UDS data structures are in an invalid state" },
  { "UDS_DUPLICATE_NAME",
    "Attempt to enter the same name into a delta index twice" },
  { "UDS_UNEXPECTED_RESULT", "Unexpected result from internal routine" },
  { "UDS_INJECTED_ERROR", "Injected error" },
  { "UDS_ASSERTION_FAILED", "Assertion failed" },
  { "UDS_INTERNAL_UNUSED_9", "Unused internal error 9" },
  { "UDS_QUEUED", "Request queued" },
  { "UDS_INTERNAL_UNUSED_11", "Unused internal error 11" },
  { "UDS_INTERNAL_UNUSED_12", "Unused internal error 12" },
  { "UDS_BUFFER_ERROR", "Buffer error" },
  { "UDS_INTERNAL_UNUSED_14", "Unused internal error 14" },
  { "UDS_INTERNAL_UNUSED_15", "Unused internal error 15" },
  { "UDS_NO_DIRECTORY", "Expected directory is missing" },
  { "UDS_CHECKPOINT_INCOMPLETE", "Checkpoint not completed" },
  { "UDS_INTERNAL_UNUSED_18", "Unused internal error 18" },
  { "UDS_INTERNAL_UNUSED_19", "Unused internal error 19" },
  { "UDS_ALREADY_REGISTERED", "Error range already registered" },
  { "UDS_BAD_IO_DIRECTION", "Bad I/O direction" },
  { "UDS_INCORRECT_ALIGNMENT", "Offset not at block alignment" },
  { "UDS_OUT_OF_RANGE", "Cannot access data outside specified limits" },
};

typedef struct errorBlock {
  const char      *name;
  int              base;
  int              last;
  int              max;
  const ErrorInfo *infos;
} ErrorBlock;

enum {
  MAX_ERROR_BLOCKS = 6          // needed for testing
};

static struct errorInformation {
  int        allocated;
  int        count;
  ErrorBlock blocks[MAX_ERROR_BLOCKS];
} registeredErrors = {
  .allocated = MAX_ERROR_BLOCKS,
  .count     = 2,
  .blocks    = {
    {
      .name  = "UDS Error",
      .base  = UDS_ERROR_CODE_BASE,
      .last  = UDS_ERROR_CODE_LAST,
      .max   = UDS_ERROR_CODE_BLOCK_END,
      .infos = errorList,
    },
    {
      .name  = "UDS Internal Error",
      .base  = UDS_INTERNAL_ERROR_CODE_BASE,
      .last  = UDS_INTERNAL_ERROR_CODE_LAST,
      .max   = UDS_INTERNAL_ERROR_CODE_BLOCK_END,
      .infos = internalErrorList,
    }
  }
};

/**
 * Fetch the error info (if any) for the error number.
 *
 * @param errnum        the error number
 * @param infoPtr       the place to store the info for this error (if known),
 *                      otherwise set to NULL
 *
 * @return              the name of the error block (if known), NULL othersise
 **/
static const char *getErrorInfo(int errnum, const ErrorInfo **infoPtr)
{

  if (errnum == UDS_SUCCESS) {
    if (infoPtr != NULL) {
      *infoPtr = &successful;
    }
    return NULL;
  }

  ErrorBlock *block;
  for (block = registeredErrors.blocks;
       block < registeredErrors.blocks + registeredErrors.count;
       ++block) {
    if ((errnum >= block->base) && (errnum < block->last)) {
      if (infoPtr != NULL) {
        *infoPtr = block->infos + (errnum - block->base);
      }
      return block->name;
    } else if ((errnum >= block->last) && (errnum < block->max)) {
      if (infoPtr != NULL) {
        *infoPtr = NULL;
      }
      return block->name;
    }
  }
  if (infoPtr != NULL) {
    *infoPtr = NULL;
  }
  return NULL;
}

/**
 * Return string describing a system error message
 *
 * @param errnum  System error number
 * @param buf     Buffer that can be used to contain the return value
 * @param buflen  Length of the buffer
 *
 * @return The error string, which may be a string constant or may be
 *         returned in the buf argument
 **/
#ifdef __KERNEL__
static const char *systemStringError(int errnum, char *buf, size_t buflen)
{
  const char *errorString = NULL;
  if ((errnum > 0) && (errnum < COUNT_OF(messageTable))) {
    errorString = messageTable[errnum];
  }

  size_t len = ((errorString == NULL)
                ? snprintf(buf, buflen, "Unknown error %d", errnum)
                : snprintf(buf, buflen, "%s", errorString));
  if (len < buflen) {
    return buf;
  }

  buf[0] = '\0';
  return "System error";
}
#else
static INLINE const char *systemStringError(int errnum, char *buf,
                                            size_t buflen)
{
  return strerror_r(errnum, buf, buflen);
}
#endif

/*****************************************************************************/
const char *stringError(int errnum, char *buf, size_t buflen)
{
  if (buf == NULL) {
    return NULL;
  }

  char *buffer = buf;
  char *bufEnd = buf + buflen;

  if (isUnrecoverable(errnum)) {
    buffer = appendToBuffer(buffer, bufEnd, "Unrecoverable error: ");
    errnum = sansUnrecoverable(errnum);
  }

  const ErrorInfo *info      = NULL;
  const char      *blockName = getErrorInfo(errnum, &info);

  if (blockName != NULL) {
    if (info != NULL) {
      buffer = appendToBuffer(buffer, bufEnd,
                              "%s: %s", blockName, info->message);
    } else {
      buffer = appendToBuffer(buffer, bufEnd,
                              "Unknown %s %d", blockName, errnum);
    }
  } else if (info != NULL) {
    buffer = appendToBuffer(buffer, bufEnd, "%s", info->message);
  } else {
    const char *tmp = systemStringError(errnum, buffer, bufEnd - buffer);
    if (tmp != buffer) {
      buffer = appendToBuffer(buffer, bufEnd, "%s", tmp);
    } else {
      buffer += strlen(tmp);
    }
  }
  return buf;
}

/*****************************************************************************/
const char *stringErrorName(int errnum, char *buf, size_t buflen)
{
  errnum = sansUnrecoverable(errnum);

  char *buffer = buf;
  char *bufEnd = buf + buflen;

  const ErrorInfo *info      = NULL;
  const char      *blockName = getErrorInfo(errnum, &info);

  if (blockName != NULL) {
    if (info != NULL) {
      buffer = appendToBuffer(buffer, bufEnd, "%s", info->name);
    } else {
      buffer = appendToBuffer(buffer, bufEnd, "%s %d", blockName, errnum);
    }
  } else if (info != NULL) {
    buffer = appendToBuffer(buffer, bufEnd, "%s", info->name);
  } else {
    const char *tmp = systemStringError(errnum, buffer, bufEnd - buffer);
    if (tmp != buffer) {
      buffer = appendToBuffer(buffer, bufEnd, "%s", tmp);
    } else {
      buffer += strlen(tmp);
    }
  }
  return buf;
}

/*****************************************************************************/
int registerErrorBlock(const char      *blockName,
                       int              firstError,
                       int              lastReservedError,
                       const ErrorInfo *infos,
                       size_t           infoSize)
{
  int result = ASSERT(firstError < lastReservedError,
                      "bad error block range");
  if (result != UDS_SUCCESS) {
    return result;
  }

  if (registeredErrors.count == registeredErrors.allocated) {
    // could reallocate and grow, but should never happen
    return UDS_OVERFLOW;
  }

  ErrorBlock *block;
  for (block = registeredErrors.blocks;
       block < registeredErrors.blocks + registeredErrors.count;
       ++block) {
    if (strcmp(blockName, block->name) == 0) {
      return UDS_DUPLICATE_NAME;
    }
    // check for overlap in error ranges
    if ((firstError < block->max) && (lastReservedError > block->base)) {
      return UDS_ALREADY_REGISTERED;
    }
  }

  registeredErrors.blocks[registeredErrors.count++] = (ErrorBlock) {
    .name  = blockName,
    .base  = firstError,
    .last  = firstError + (infoSize / sizeof(ErrorInfo)),
    .max   = lastReservedError,
    .infos = infos
  };

  return UDS_SUCCESS;
}
