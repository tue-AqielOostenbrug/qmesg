# qmesg - opensource IRCv3 messaging
## Documentation
Please consult the Doxygen [pdf](/docs/latex/refman.pdf) or the [online guide](https://tue-aqieloostenbrug.github.io/qmesg/html/)

## [Linux] How to use the client
First compile the client as follows:
```bash
make
```
```bash
make linux
```
Now execute the binary:
```bash
./linux.o
```
## [Linux] How to recompile the client
```bash
make clean
```
```bash
make
```
```bash
make linux
```

## [Linux] How to test with server
Assuming Docker has been installed.

The server can be started using:
```bash
make server
```
Inside of the container then run:
```bash
./unrealircd start
```
*You now connect to `127.0.0.1:6667` using the client.*

## [Linux] How to generate the documentation
Assuming doxygen has been installed.

The documentation website and pdf can be compiled as follows:
```bash
make docs
```
## Bugs
- [Authentication Race Condition (Linux) #3](https://github.com/tue-AqielOostenbrug/qmesg/issues/3#issue-3693864198)
- [Unexpected cuts in responses from the server #5](https://github.com/tue-AqielOostenbrug/qmesg/issues/5#issue-3694024591)
- Seems to get stuck in a livelock somewhere resulting in not being able to input commands (currently no examples)
