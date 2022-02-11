# dedup
 
remove non needed duplicates from your machine, currently tested only for Mac osx

```shell
brew install openssl
cd /path/to/clone-of-dedup
mkdir build
cd build
cmake ..
make
./dedup directory-susepected-of-having-duplicated-files
i.e:
./dedup /home/you/Downloads
```

