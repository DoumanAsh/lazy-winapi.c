#!/bin/bash
REPO=`git config remote.origin.url`

rm -rf html/
git clone $REPO html
cd html/
git checkout gh-pages || git checkout --orphan gh-pages
rm -rf *

cd -

doxygen Doxygen.config

SHA=`git log -1 --format="%s(%h %cd)" --date=short`

cd html/

if [[ -z `git status --porcelain` ]]; then
    echo "No changes to the docs"
    exit 0
fi

git add .
git commit -m "Auto-update" -m "Commit: ${SHA}"
git push origin HEAD

cd -

rm -rf html/
