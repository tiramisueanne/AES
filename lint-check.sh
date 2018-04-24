clang-format -i -style=google *.cc
clang-format -i -style=google *.h
cc_diff="$( git diff *.cc )"
h_diff="$( git diff *.h )"

if [ "$cc_diff" == "" ] && [ "$h_diff" == "" ]
then
  exit 0
fi

git checkout -- *.cc
git checkout -- *.h

echo "Code is not linted. Please lint and commit."
exit 1
