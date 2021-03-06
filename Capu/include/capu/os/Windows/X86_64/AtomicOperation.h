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

#ifndef CAPU_WINDOWS_X86_64_PLATFORMOPERATION_H
#define CAPU_WINDOWS_X86_64_PLATFORMOPERATION_H

#include <capu/os/Windows/AtomicOperation.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class AtomicOperation: private os::AtomicOperation
            {
            public:
                using os::AtomicOperation::AtomicLoad;
                using os::AtomicOperation::AtomicStore;
                using os::AtomicOperation::AtomicAdd;
                using os::AtomicOperation::AtomicSub;
                using os::AtomicOperation::AtomicInc;
                using os::AtomicOperation::AtomicDec;
            };
        }
    }
}

#endif // CAPU_WINDOWS_X86_64_PLATFORMOPERATION_H
