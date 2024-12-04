# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/santi/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "C:/esp32/second_work_led/build/bootloader"
  "C:/esp32/second_work_led/build/bootloader-prefix"
  "C:/esp32/second_work_led/build/bootloader-prefix/tmp"
  "C:/esp32/second_work_led/build/bootloader-prefix/src/bootloader-stamp"
  "C:/esp32/second_work_led/build/bootloader-prefix/src"
  "C:/esp32/second_work_led/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/esp32/second_work_led/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/esp32/second_work_led/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
