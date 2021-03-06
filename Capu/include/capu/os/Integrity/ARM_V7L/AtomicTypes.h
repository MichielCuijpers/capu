/*
 * Copyright (C) 2016 BMW Car IT GmbH
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

#ifndef CAPU_INTEGRITY_ARM_V7L_ATOMICTYPES_H
#define CAPU_INTEGRITY_ARM_V7L_ATOMICTYPES_H

#include "capu/Config.h"
#include <pthread.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class AtomicBool
            {
            public:
                bool load() const;
                void store(bool desired);
            private:
                volatile mutable Address mValue;
            };

            inline
            bool AtomicBool::load() const
            {
                const Address Zero = 0;
                Address oldValue;
                AtomicModify(&mValue, &oldValue, Zero, Zero);
                return oldValue != Zero;
            }

            inline
            void AtomicBool::store(bool desired)
            {
                const Address setValue = static_cast<Address>( desired ? 1 : 0 );
                const Address FF       = ~0;
                Address       oldValue;
                AtomicModify(&mValue, &oldValue, FF, setValue);
            }
        }
    }
}

#endif // CAPU_INTEGRITY_ARM_V7L_ATOMICTYPES_H
