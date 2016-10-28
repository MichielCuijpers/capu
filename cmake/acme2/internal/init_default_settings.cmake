############################################################################
#
# Copyright (C) 2014 BMW Car IT GmbH
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

SET(ACME2_DEFAULT_VALUES

    # Project metadata
    NAME                    "default_name"
    TYPE                    STATIC_LIBRARY
    DESCRIPTION             "default_description"
    URL                     "default_url"
    COMPANY                 "default_company"
    EMAIL                   "default_email"

    # version
    VERSION_MAJOR           0
    VERSION_MINOR           0
    VERSION_PATCH           0
    VERSION_STRING          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"

    # source paths
    INCLUDE_BASE            include
    FILES_PUBLIC_HEADER     #include/*.h
    FILES_PRIVATE_HEADER    #src/*.h
    FILES_SOURCE            #src/*.h
    FILES_DOCUMENTATION     #doc/*
    FILES_RESOURCE          #res/*
    FILES_GENERATED

    # dependencies
    DEPENDENCIES
    DEPENDENCIES_HEADER
    DEPENDENCIES_SYSTEM

    # install module: boolean
    ENABLE_INSTALL          ON

    # install paths
    INSTALL_HEADER          include
    INSTALL_BINARY          bin
    INSTALL_TEST            bin
    INSTALL_TEST_RESOURCE   bin/res
    INSTALL_STATIC_LIB      lib
    INSTALL_SHARED_LIB      lib
    INSTALL_PLUGIN          bin
    INSTALL_RESOURCE        bin/res
    INSTALL_DOCUMENTATION   share/doc/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    INSTALL_CMAKEFINDMODULE lib/cmake
    INSTALL_PKGCONFIG       lib/pkgconfig

    # content
    CONTENT
)

#provide variables DEFAULT_<ACME2_API>
ARGUMENT_SPLITTER("${ACME2_DEFAULT_VALUES}" "${ACME2_API}" DEFAULT_)
