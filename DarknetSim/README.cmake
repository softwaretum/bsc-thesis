mkdir build && cd build && cmake ..
Should work fine. Some configurations might require interaction.
When it's done, CMake will have generated a GNU Makefile in the build-directory.
CMake can also generate other files; Invoke cmake without arguments or see the Generators-section of the manpage to see what's available.
To build in a specific configuration, invoke cmake with -D CMAKE_BUILD_TYPE=<configuration>. See the CMAKE_BUILD_TYPE-entry in the manpage for cmake for available configurations.
Additionally, this CMake-file can provide additional warnings (it does so by default). You can disable this by invoking cmake with -D BUILD_EXTRA_WARNINGS=no. And you can get even more warnings with -D BUILD_EXTRA_WARNINGS=Warn-more.
You can use different compilers simply by providing the CC and CXX environmet-variables.

Add source-files to src/CMakeLists.txt below the add_executable-line.
OMNeT++-message files, which have to be preprocessed, go below the convert_message_files-line of the same file.
