/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_LINUX_SIGNAL_H
#define CAPU_LINUX_SIGNAL_H

namespace capu
{
    enum ESignal
    {
        CAPU_SIGABRT = SIGABRT,
        CAPU_SIGFPE = SIGFPE,
        CAPU_SIGILL = SIGILL,
        CAPU_SIGINT = SIGINT,
        CAPU_SIGSEGV = SIGSEGV,
        CAPU_SIGTERM = SIGTERM
    };

    namespace os
    {
        class Signal
        {
        public:
            typedef void (*SignalHandlerFunction)(int32_t);

            static SignalHandlerFunction signal(ESignal /*sig*/, SignalHandlerFunction /*func*/)
            {
                return SignalHandlerFunction(0);
            }

            /// Generates a signal.
            /** @param sig The signal.
             *  @return Returns CAPU_OK when successful, and CAPU_ERROR otherwise. */
            static status_t raise(ESignal sig)
            {
                return CAPU_ERROR;
            }
        };
    }
}
#endif // CAPU_ANDROID_SIGNAL_H
