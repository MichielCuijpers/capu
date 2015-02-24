/*
 * Copyright (C) 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAPU_ERROR_H
#define CAPU_ERROR_H

#include "Config.h"

namespace capu
{
    /**
     * Type of status codes of capu functions
     */
    typedef int32_t status_t;

    /**
     * Error codes of capu functions
     */
    enum Errors
    {
        CAPU_OK = 0,
        CAPU_EUNIMPL = 1,
        CAPU_ERANGE = 2,
        CAPU_EINVAL = 3,
        CAPU_ERROR = 4,
        CAPU_SOCKET_EBIND = 5,
        CAPU_SOCKET_ESOCKET = 6,
        CAPU_SOCKET_ECONNECT = 7,
        CAPU_SOCKET_ELISTEN = 8,
        CAPU_SOCKET_ECLOSE = 9,
        CAPU_SOCKET_EADDR = 10,
        CAPU_ENO_MEMORY = 11,
        CAPU_ETIMEOUT = 12,
        CAPU_ENOT_EXIST = 13,
        CAPU_ENOT_SUPPORTED = 14,
        CAPU_EIO = 15,
        CAPU_EOF = 16,
        CAPU_INTERRUPTED = 17
    };
}
#endif //CAPU_ERROR_H

