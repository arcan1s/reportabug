#!/bin/bash

ARCHIVE="reportabug"
SRCDIR="sources"
FILES="AUTHORS CHANGELOG COPYING README.md"
IGNORELIST="build *.cppcheck *.qm"
VERSION=$(grep -m1 PROJECT_VERSION_MAJOR sources/CMakeLists.txt | awk '{print $3}' | cut -c 1).\
$(grep -m1 PROJECT_VERSION_MINOR sources/CMakeLists.txt | awk '{print $3}' | cut -c 1).\
$(grep -m1 PROJECT_VERSION_PATCH sources/CMakeLists.txt | awk '{print $3}' | cut -c 1)
# create archive
[[ -e ${ARCHIVE}-${VERSION}-src.tar.xz ]] && rm -f "${ARCHIVE}-${VERSION}-src.tar.xz"
[[ -d ${ARCHIVE} ]] && rm -rf "${ARCHIVE}"
cp -r "${SRCDIR}" "${ARCHIVE}"
for FILE in ${FILES[*]}; do cp -r "$FILE" "${ARCHIVE}"; done
for FILE in ${IGNORELIST[*]}; do find "${ARCHIVE}" -name "${FILE}" -exec rm -rf {} \;; done
tar cJf "${ARCHIVE}-${VERSION}-src.tar.xz" "${ARCHIVE}"
rm -rf "${ARCHIVE}"
