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
- Run cmake, by default it will use gcc compiler 
``` sh
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CPP_COMPILER=g++ -S . -B build
```
- You can specify other compilter (clang, clang++) 
``` sh
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CPP_COMPILER=clang++ -S . -B build
```
- Enable DEBUG 

``` sh
cmake -DDEBUG=ON -S . -B build
```
- Then run make 
``` sh
cmake --build build
```
- Add option VERBOSE=1 to view more log
``` sh
make VERBOSE=1 
```

## References
- [HTTP 1.1 RFC](https://datatracker.ietf.org/doc/html/rfc7230)
