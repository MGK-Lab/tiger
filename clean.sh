cd "$(find ~/ -iname tiger)"
find -iname '*lib*' | xargs rm -rf
make clean

