# qmesg - opensource IRCv3 messaging
## Documentation
Please consult the Doxygen [pdf](/latex/refman.pdf) or the [online guide](https://tue-aqieloostenbrug.github.io/qmesg/html/)

## [Linux] How to use
```bash
make
```
```bash
make linux
```
```bash
./linux.o
```
## How to recompile
```bash
make clean
```
```bash
make
```
```bash
make linux
```

## Bugs
- [Possible race conditions during authentication resulting in misses. (maybe sends to fast)](src/linux/main.c)
- [Unexpected cuts in responses from the server](src/linux/main.c)
- [Seems to get stuck in a livelock somewhere resulting in not being able to input commands](src/linux/main.c)
