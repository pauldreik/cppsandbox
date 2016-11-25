#!/bin/sh
#
# executes clang format on all .h and .cpp files outside directory "build",
# in the working directory.
#
# by Paul Dreik https://www.pauldreik.se/
# LICENSE: http://www.boost.org/LICENSE_1_0.txt

 
#find clang format (usually named clang-format-3.x or clang-format, who knows)
CLANGFORMAT=$(find /usr/local/bin /usr/bin -executable -name "clang-format*" |grep -v -- -diff |sort -g |tail -n1)

if [ ! -x "$CLANGFORMAT" ] ; then
    echo failed finding clangformat
    exit 1
else
    echo found clang format: $CLANGFORMAT
fi

find . -path ./build -prune -o -type f \( -name "*.h" -o -name "*.cpp" \) -print0 | \
    xargs -0 -n1 $CLANGFORMAT -i
