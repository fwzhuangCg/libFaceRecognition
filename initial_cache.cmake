# Boost:
# -------
# Windows: Download the pre-built binaries from http://sourceforge.net/projects/boost/files/boost-binaries/ for VS2013 (msvc-12 64bit).

# Set the windows PATH variable to "<YOUR_BOOST_DIRECTORY>\lib64-msvc-12.0" and CMake will find it.
# Linux: Boost can usually be installed via a package manager (e.g. apt-get install boost-all-dev) and this variable can be left uncommented.
#set(BOOST_ROOT     "/home/user/boost/install"    CACHE STRING "Boost search location" FORCE)


# Other non-CMake libraries
# ==============================
# FaceVACS SDK:
# -------

# Windows: Set directories of the release and debug dlls and libs, and headers.
set(FVSDK_LIB_DIR "C:\\FVSDK_8_9_5\\lib\\x86_64\\msc_12.0-ipp_crtdll\\libfrsdk-8.9.5" CACHE STRING "Directory of the FVSDK release dll and lib files" FORCE)
set(FVSDK_DBG_LIB_DIR "C:\\FVSDK_8_9_5\\lib\\x86_64\\msc_12.0-ipp_crtdll_g\\libfrsdk-8.9.5d" CACHE STRING "Directory of the FVSDK debug dll and lib files" FORCE)
set(FVSDK_INCLUDE_DIR "C:\\FVSDK_8_9_5\\include" CACHE STRING "Directory of the FVSDK headers" FORCE)

# Linux: Only one directory for the libs, plus the headers.
#set(FVSDK_LIB_DIR "/home/student/cognitecSDK/FVSDK_8_9_5/lib/x86_64/gcc-4.6-ipp" CACHE STRING "Directory of the FVSDK .so libraries" FORCE)
#set(FVSDK_INCLUDE_DIR "/home/student/cognitecSDK/FVSDK_8_9_5/include" CACHE STRING "Directory of the FVSDK headers" FORCE)

#OpenCV build path
set(OpenCV_DIR   "C:\\opencv\\build"   CACHE STRING "OpenCV config dir, where OpenCVConfig.cmake can be found" FORCE)