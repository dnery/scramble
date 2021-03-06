/*
 *   Copyright 2012 Thomas Dalling.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#include "platform.hh"

#include <cmath>
#include <limits>
#include <climits>

#if defined( __APPLE_CC__ ) || defined ( __APPLE__ )
        #define PLATFORM_OSX
        #include <CoreFoundation/CoreFoundation.h>
#elif defined( __WIN32__ ) || defined( _WIN32 ) || defined( _WIN64 )
        #define PLATFORM_WIN32
        #include <windows.h>
#elif defined ( linux ) || defined( __linux__ )
        #define PLATFORM_LINUX
        #include <libgen.h>
        #include <unistd.h>
#elif defined( __HAIKU__ ) || defined( __BEOS__ )
        #define PLATFORM_HAIKU
        #include <kernel/OS.h>
        #include <kernel/image.h>
#elif defined( __SVR4 )
        #define PLATFORM_SOLARIS
        #include <stdlib.h>
#elif defined( __FreeBSD__ ) || defined(__OpenBSD__) || defined( __NetBSD__ ) || defined( __DragonFly__ )
        #define PLATFORM_BSD
        #include <sys/sysctl.h>
#endif

static std::string process_path() {
#if defined( PLATFORM_OSX )
        char exe_file[PATH_MAX + 1];
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        if (mainBundle) {
                CFURLRef mainURL = CFBundleCopyBundleURL(mainBundle);

                if (mainURL) {
                        int ok = CFURLGetFileSystemRepresentation ( mainURL, (Boolean) true, (UInt8*)exe_file, PATH_MAX );

                        if (ok) {
                                return std::string(exe_file) + "/";
                        }
                }
        }

        return "./";
#elif defined( PLATFORM_LINUX )
        char exe_file[PATH_MAX + 1];
        int size;
        size = (int) readlink("/proc/self/exe", exe_file, PATH_MAX);
        if (size < 0) {
                return "./";
        } else {
                exe_file[size] = '\0';
                return std::string(dirname(exe_file)) + "/";
        }
#elif defined( PLATFORM_WIN32 )
        // Get path to executable:
        TCHAR szDllName[_MAX_PATH];
        TCHAR szDrive[_MAX_DRIVE];
        TCHAR szDir[_MAX_DIR];
        TCHAR szFilename[_MAX_DIR];
        TCHAR szExt[_MAX_DIR];
        GetModuleFileName(0, szDllName, _MAX_PATH);

        _splitpath(static_cast<const char *>(szDllName),
                        static_cast<char *>(zDrive),
                        static_cast<char *>(szDir),
                        static_cast<char *>(szFilename),
                        static_cast<char *>(szExt);

        return std::string(static_cast<char *>(zDrive) + std::string(static_cast<char *>(zDir));
#elif defined( PLATFORM_BSD )
        int mib[4];
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PATHNAME;
        mib[3] = -1;
        char buf[1024];
        size_t cb = sizeof(buf);
        sysctl(mib, 4, buf, &cb, NULL, 0);

        return FileRemoveFileName( std::string( buf ) );
#elif defined( PLATFORM_SOLARIS )
        return FileRemoveFileName( std::string( getexecname() ) );
#elif defined( PLATFORM_HAIKU )
        image_info info;
        int32 cookie = 0;

        while ( B_OK == get_next_image_info( 0, &cookie, &info ) ) {
                if ( info.type == B_APP_IMAGE )
                        break;
        }

        return FileRemoveFileName( std::string( info.name ) );
#else
        #warning GetProcessPath() not implemented on this platform. ( will return "./" )
        return "./";
#endif
}

std::string resource_path(std::string file_name) {
        return process_path() + "../assets/" + file_name;
}

/*
 * Tampering notice:
 *
 * Originally maintained by Thomas Dalling (http://www.tomdalling.com/),
 * this file has been modified and redistributed by me (dznery@gmail.com).
 */
