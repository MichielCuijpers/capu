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

#ifndef CAPU_POSIX_FILEUTILS_H
#define CAPU_POSIX_FILEUTILS_H

#include "capu/Config.h"
#include "capu/os/File.h"

#include <unistd.h>
#include <limits.h>

namespace capu
{
    namespace posix
    {
        class FileUtils
        {
        public:
            static capu::File getCurrentWorkingDirectory();
            static status_t setCurrentWorkingDirectory(const capu::File& directory);
        };

        inline
        capu::File FileUtils::getCurrentWorkingDirectory()
        {
            char cwd[PATH_MAX];
            char* ret = getcwd(cwd, sizeof(cwd));
            if(ret != NULL)
            {
                capu::File file(cwd);
                return file;
            }
            return capu::File("");
        }

        inline
        status_t FileUtils::setCurrentWorkingDirectory(const capu::File& directory)
        {
            if(0 == chdir(directory.getPath().c_str()))
            {
                return CAPU_OK;
            }
            return CAPU_ERROR;
        }
    }
}

#endif //CAPU_POSIX_FILEUTILS_H
