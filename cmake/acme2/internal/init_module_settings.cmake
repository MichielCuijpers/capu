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

# translate all MODULE_* provided by ACME2 module
# to checked and preprocessed ACME_* variables

IF (POLICY CMP0054)
    CMAKE_POLICY(SET CMP0054 NEW)
ENDIF()

# reset MODULE_<value>
FOREACH(PROPERTY ${ACME2_API})
    SET(MODULE_${PROPERTY} "")
ENDFOREACH()

# values provided by ACME_PROJECT call are in PROJECT_<value>

# values provided by ACME_MODULE call are in MOUDLE_<value>
ARGUMENT_SPLITTER("${MODULE_SETTINGS}" "${ACME2_API}" MODULE_)

# apply module settings or use project values,
# if property was was not provided
FOREACH(PROPERTY ${ACME2_API})
    IF("${MODULE_${PROPERTY}}" STREQUAL "")
        SET(MODULE_${PROPERTY} ${PROJECT_${PROPERTY}})
    ENDIF()
ENDFOREACH()

# do not use CONTENT, this is project specific
SET(MODULE_CONTENT      "")

# if project has ${PROJECT_NAME}_INSTALL_TESTS set,
# force install of unit tests for this module
IF("${MODULE_TYPE}" STREQUAL "TEST")
    IF(${${PROJECT_NAME}_INSTALL_TESTS})
        SET(MODULE_ENABLE_INSTALL TRUE)
    ENDIF()
ENDIF()
