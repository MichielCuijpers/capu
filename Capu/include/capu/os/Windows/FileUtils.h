/*
 * Copyright (C) 2014 BMW Car IT GmbH
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

#ifndef CAPU_WINDOWS_FILEUTILS_H
#define CAPU_WINDOWS_FILEUTILS_H

#include "capu/Config.h"
#include "capu/os/File.h"

namespace capu
{
    namespace os
    {
        class FileUtils
        {
        public:
            static capu::File getCurrentWorkingDirectory();
            static status_t setCurrentWorkingDirectory(const capu::File& directory);
        };
    }
}

#endif //CAPU_WINDOWS_FILEUTILS_H
