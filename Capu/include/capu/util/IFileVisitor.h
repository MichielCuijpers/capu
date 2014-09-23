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

#ifndef CAPU_IFILEVISITOR_H
#define CAPU_IFILEVISITOR_H

#include "capu/Error.h"
#include "capu/Config.h"

namespace capu
{
    class File;

    /**
     * Interface for a file visitor.
     */
    class IFileVisitor
    {
    public:

        /**
        * Called every time a file is visited.
        * @param file The file.
        * @param stepIntoDirectory Allows an IFileVisitor to specify weather the walk should continue inside a directory. Default value is true. Only applies for directories.
        * @return CAPU_ERROR to stop traversal. CAPU_OK otherwise.
        */
        virtual status_t visit(File& file, bool_t& stepIntoDirectory) = 0;
    };
}

#endif // CAPU_IFILEVISITOR_H
