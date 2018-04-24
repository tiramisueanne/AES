#!bin/bash

cpplint *.cc
cpplint *.h

cc_diff="$(cpplint --quiet *.cc 2>&1)"
h_diff="$(cpplint --quiet *.h 2>&1)"
# { cc_diff=$(cpplint *.cc 2>&1 1>&3-) ;} 3>&1
# { h_diff=$(cpplint *.h 2>&1 1>&3-) ;} 3>&1

if [ "$cc_diff" == "" ] && [ "$h_diff" == "" ]
then
  echo
  echo "Code linted properly."
  exit 0
fi

echo
echo "Code is not linted. Please lint and commit."
exit 1
