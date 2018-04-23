# Installing Google Test

## Mac

1. Before installation, ensure that CMake is installed on your machine. We use [brew](https://brew.sh/) to install Mac packages.
```
$ brew install cmake
```

2. [Download the gtest file](https://github.com/google/googletest/archive/release-1.8.0.zip) from the Google Test repo and unzip it. (You can view all of the repo's releases [here](https://github.com/google/googletest/releases).)
```
$ curl -# -L -o "googletest-release-1.8.0.zip" "https://github.com/google/googletest/archive/release-1.8.0.zip"
$ unzip googletest-release-1.8.0.zip
```

3. Complete the installation
```
$ cd googletest-release-1.8.0/googletest/
$ mkdir bld
$ cd bld
$ cmake ..
$ make
$ cp -a ../include/gtest /usr/local/include
$ cp -a *.a /usr/local/lib
```

4. Clean up by deleting the  ```googletest-release-1.8.0``` folder and ```googletest-release-1.8.0.zip```
```
$ # Assuming you're still in the bld directory
$ cd ../../..
$ rm -r googletest-release-1.8.0
$ rm googletest-release-1.8.0.zip
```

Instructions courtesy of [Zhiwei](https://stackoverflow.com/users/6580318/zhiwei) on [StackOverflow](https://stackoverflow.com/a/42707478).
