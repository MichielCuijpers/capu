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

#ifndef CAPU_WINDOWS_TCP_SOCKET_H
#define CAPU_WINDOWS_TCP_SOCKET_H

#include "capu/os/Socket.h"
#include "capu/os/Generic/TcpSocket.h"

namespace capu
{
    namespace os
    {

        class TcpSocket : private capu::generic::TcpSocket
        {
        public:
            friend class TcpServerSocket;

            TcpSocket();
            TcpSocket(const SocketDescription& socketDescription);
            ~TcpSocket();

            status_t send(const char* buffer, int32_t length, int32_t& sentBytes);
            status_t receive(char* buffer, int32_t length, int32_t& numBytes);
            status_t close();
            status_t connect(const char* dest_addr, uint16_t port);

            status_t setTimeout(int32_t timeout);
            status_t getTimeout(int32_t& timeout);

            using capu::generic::TcpSocket::setBufferSize;
            using capu::generic::TcpSocket::setLingerOption;
            using capu::generic::TcpSocket::setNoDelay;
            using capu::generic::TcpSocket::setKeepAlive;
            using capu::generic::TcpSocket::getBufferSize;
            using capu::generic::TcpSocket::getLingerOption;
            using capu::generic::TcpSocket::getNoDelay;
            using capu::generic::TcpSocket::getKeepAlive;
            using capu::generic::TcpSocket::getRemoteAddress;
            using capu::generic::TcpSocket::getSocketDescription;

        protected:
        private:
            WSADATA mWsaData;
            int32_t mTimeout;

            status_t initializeSocket();
            status_t setWindowsSocketParams();
            status_t setTimeoutInternal();
        };

        inline
        TcpSocket::TcpSocket()
        : mTimeout(-1)
        {
        }

        inline
        TcpSocket::TcpSocket(const SocketDescription& socketDescription)
        : capu::generic::TcpSocket()
        , mTimeout(-1)
        {
            //Initialize Winsock
            int32_t result = WSAStartup(MAKEWORD(2, 2), &mWsaData);
            if (result == 0)
            {
                //create the socket which is used to connect the server
                mSocket = socketDescription;
                if (mSocket == INVALID_SOCKET)
                {
                    WSACleanup();
                }
            }
            else
            {
                mSocket = INVALID_SOCKET;
            }
        }

        inline
        status_t TcpSocket::initializeSocket()
        {
            status_t status = CAPU_OK;
            int32_t result = WSAStartup(MAKEWORD(2, 2), &mWsaData);
            if (0 == result)
            {
                //create the socket which is used to connect the server
                mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (mSocket == INVALID_SOCKET)
                {
                    WSACleanup();
                    status = CAPU_SOCKET_ESOCKET;
                }
            }
            else
            {
                mSocket = INVALID_SOCKET;
                status = CAPU_SOCKET_ESOCKET;
            }

            return status;
        }

        inline
        status_t TcpSocket::setWindowsSocketParams()
        {
            status_t returnStatus = CAPU_OK;
            status_t status = setSocketParameters();
            if (status != CAPU_OK) {
                returnStatus = status;
            }

            status = setTimeoutInternal();
            if (status != CAPU_OK) {
                returnStatus = status;
            }

            return returnStatus;
        }



        inline
        status_t TcpSocket::connect(const char* dest_addr, uint16_t port)
        {
            status_t status = initializeSocket();
            if (status != CAPU_OK)
            {
                return status;
            }

            status = setWindowsSocketParams();
            if (status != CAPU_OK && status != CAPU_EINVAL)
            {
                return status;
            }


            //check parameters
            if ((dest_addr == NULL) || (port == 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct sockaddr_in serverAddress;
            status = getSocketAddr(dest_addr, port, serverAddress);
            if (status != CAPU_OK) 
            {
                return status;
            }

            if (mTimeout == 0)
            {
                unsigned long nonblocking = 1;
                ioctlsocket(mSocket, FIONBIO, &nonblocking);
            }

            if (::connect(mSocket, (sockaddr*) &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
            {
                close();
                return CAPU_SOCKET_ECONNECT;
            }

            if (mTimeout == 0)
            {
                unsigned long nonblocking = 0;
                ioctlsocket(mSocket, FIONBIO, &nonblocking);
            }

            return CAPU_OK;
        }

        inline TcpSocket::~TcpSocket()
        {
            close();
        }

        inline status_t TcpSocket::send(const char* buffer, int32_t length, int32_t& sentBytes)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            const int32_t result = ::send(mSocket, buffer, length, 0);
            if (result == SOCKET_ERROR)
            {
                int lastError = WSAGetLastError();
                switch(lastError)
                {
                case WSAETIMEDOUT:
                    // When getting a timeout on send on Windows Socket,s the MSDN documentation http://msdn.microsoft.com/en-us/library/ms740476
                    // says that the "socket state is indeterminate, and should not be used" anymore. Therefore we report an error here.
                    // This timeout on send is especially seen in case the TCP receive window gets zero. In this case some Windows implementations
                    // return this timeout error and send a TCP package with the RST flag to the remote peer. This causes a connection reset on the 
                    // remote site, so the connection cannot be used anymore.
                    close();
                    return CAPU_ERROR;
                case WSAEINTR:
                    return CAPU_OK;
                default:
                    close();
                    return CAPU_ERROR;
                }
            }

            sentBytes = result;
            return CAPU_OK;
        }

        inline status_t TcpSocket::receive(char* buffer, int32_t length, int32_t& numBytes)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            const int32_t result = recv(mSocket, buffer, length, 0);
            if (result == SOCKET_ERROR)
            {
                numBytes = 0;
                const int32_t error = WSAGetLastError();
                if (error == WSAETIMEDOUT)
                {
                    return CAPU_ETIMEOUT;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }
            numBytes = result;
            return CAPU_OK;
        }

        inline status_t TcpSocket::close()
        {
            int32_t returnValue = CAPU_OK;
            if (mSocket == INVALID_SOCKET)
            {
                returnValue = CAPU_SOCKET_ESOCKET;
            }
            else
            {
                shutdown(mSocket, SD_BOTH);
                int32_t result = closesocket(mSocket);
                if (result != 0)
                {
                    result = WSAGetLastError();
                    if (result != WSANOTINITIALISED)  //socket has already been closed
                    {
                        returnValue = CAPU_SOCKET_ECLOSE;
                    }
                }
                WSACleanup();
            }
            mSocket = INVALID_SOCKET;
            return returnValue;
        }

        inline
        status_t
        TcpSocket::setTimeout(int32_t timeout)
        {
            mTimeout = timeout;
            if (CAPU_INVALID_SOCKET != mSocket)
            {
                return setTimeoutInternal();
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setTimeoutInternal()
        {
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&mTimeout, sizeof(mTimeout)) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&mTimeout, sizeof(mTimeout)) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::getTimeout(int32_t& timeout)
        {
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                timeout = mTimeout;
                return CAPU_OK;
            }

            struct timeval soTimeout;
            socklen_t len = sizeof(soTimeout);

            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&soTimeout, &len) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            timeout = soTimeout.tv_sec;

            return CAPU_OK;
        }
    }
}

#endif //CAPU_WINDOWS_TCP_SOCKET_H
