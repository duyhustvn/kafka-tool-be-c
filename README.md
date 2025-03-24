# README
## Requirements 
- Install ceedling 
``` sh
sudo apt-get install ruby-full
sudo gem install ceedling
```

## Run all test 
``` sh
ceedling test:all
```

## Build project 
### Using Cmake 
- Create directory build 
``` sh
mkdir -p build
```
- Go to build folder
``` sh
cd build
```
- Run cmake, by default it will use gcc compiler 
``` sh
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CPP_COMPILER=g++ ..
```
- You can specify other compilter (clang, clang++) 
``` sh
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CPP_COMPILER=clang++ ..
```
- Then run make 
``` sh
make
```
- Add option VERBOSE=1 to view more log
``` sh
make VERBOSE=1 
```

## References
- [HTTP 1.1 RFC](https://datatracker.ietf.org/doc/html/rfc7230)
