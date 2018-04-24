#!bin/bash

cpplint *.cc
cpplint *.h

cc_diff="$(cpplint --quiet *.cc 2>&1)"
h_diff="$(cpplint --quiet *.h 2>&1)"

if [ "$cc_diff" == "" ] && [ "$h_diff" == "" ]
then
  echo
  echo "Code linted properly."
  exit 0
fi

echo
echo "Code is not linted. Please lint and commit."
exit 1
