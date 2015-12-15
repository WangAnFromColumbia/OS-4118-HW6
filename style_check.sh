#!/bin/bash

git diff 18cb0432f97f577e66592425b799d6747ef3984d -- '*.c' '*.h' '*.S' > srcdiff
checkpatch.pl --ignore FILE_PATH_CHANGES -terse --no-signoff -no-tree --summary-file srcdiff
rm srcdiff
