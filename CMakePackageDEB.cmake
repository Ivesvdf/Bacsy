#RPM Specific options
set(CPACK_DEBIAN_PACKAGE_VERSION "${BACSY_VERSION_MAJOR}.${BACSY_VERSION_MINOR}.${BACSY_VERSION_PATCH}.-1")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "nathansamson@gmail.com")

# No dependency on poco. Its best that the package is compiled
# with a static poco that has some bugs fixed. The poco in ubuntu
# 11.04 is too old anyway.
set(CPACK_DEBIAN_PACKAGE_DEPENDS "librsync1 (>= 0.9.7)")
set(CPACK_DEBIAN_PACKAGE_SECTION "network")
