/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#ifndef CAPU_CONSOLE_H
#define CAPU_CONSOLE_H

#include <capu/Config.h>

#include <capu/os/PlatformInclude.h>
#include CAPU_PLATFORM_INCLUDE(Console)


namespace capu
{
    /**
     * Allows platform independent handling of console IO
     */
    class Console
    {
    public:

        enum ConsoleColor
        {
            RED = 0,
            BLUE,
            GREEN,
            YELLOW,
            WHITE,
            AQUA
        };

        /**
         * Prints a message on the console
         * @param format of the message (like printf)
         * @param parameters for the format (like prints)
         */
        static void Print(const char* format, ...);

        /**
        * Prints a message on the console with the given color
        * @param color for the message
        * @param format of the message (like printf)
        * @param parameters for the format (like prints)
        */
        static void Print(ConsoleColor color, const char* format, ...);

        /**
         * Checks whether there is input on stdin available or not
         * @return true if input on stdin is available, false otherwise
         */
        static bool IsInputAvailable();

        /**
         * Reads a single char from stdin
         * @param buffer on success a character is read into provided variable
         * @return CAPU_OK on success,
         *         CAPU_INTERRUPT on interrupt,
         *         CAPU_EOF if stdin is closed,
         *         CAPU_ERROR when other errors ocurred
         */
        static status_t ReadChar(char& buffer);

        /**
         * Flush the contents of stdout and stderr.
         */
        static void Flush();

        /**
         * Interrupts a console read (call to Console::ReadChar)
         *
         * Calling InterruptReadChar before ReadChar will cause the next call to ReadChar not to block.
         * If another call is currently in the blocking read, that will abort and give back -1.
         * Calling InterruptReadChar from other threads than ReadChar is safe.
         */
        static void InterruptReadChar();
    };

    inline
    bool
    Console::IsInputAvailable()
    {
        return capu::os::arch::Console::IsInputAvailable();
    }

    inline
    status_t
    Console::ReadChar(char& buffer)
    {
        return capu::os::arch::Console::ReadChar(buffer);
    }

    inline
    void
    Console::InterruptReadChar()
    {
        capu::os::arch::Console::InterruptReadChar();
    }

    inline
    void
    Console::Print(const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        capu::os::arch::Console::Print(format, argptr);
        va_end(argptr);
    }

    inline
    void 
    Console::Print(ConsoleColor color, const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        capu::os::arch::Console::Print(color, format, argptr);
        va_end(argptr);
    }

    inline
    void
    Console::Flush()
    {
        capu::os::arch::Console::Flush();
    }
}

#endif // CAPU_CONSOLE_H
