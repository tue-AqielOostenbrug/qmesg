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
- [Authentication Race Condition (Linux) #3](https://github.com/tue-AqielOostenbrug/qmesg/issues/3#issue-3693864198)
- [Unexpected cuts in responses from the server #5](https://github.com/tue-AqielOostenbrug/qmesg/issues/5#issue-3694024591)
- Seems to get stuck in a livelock somewhere resulting in not being able to input commands (currently no examples)
