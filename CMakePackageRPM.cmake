#RPM Specific options
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3+")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Internet")
set(CPACK_RPM_PACKAGE_VENDOR "Nathan Samson <nathansamson@gmail.com>")
set(CPACK_RPM_PACKAGE_VERSION 2)
# TODO: Update the version once timer bugs are fixed and new version
# is in repositories.
set(CPACK_RPM_PACKAGE_REQUIRES "poco-foundation >= 1.4.1p1, poco-net >= 1.4.1p1, poco-util >= 1.4.1p1, libstdc++ >= 4.6")
