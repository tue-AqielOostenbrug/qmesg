SUBDIRS := $(shell find src -type f -name Makefile -exec dirname {} \;)

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

clean: $(SUBDIRS)
	rm *.o

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

linux:
	gcc src/linux/*.o src/shared/*.o -o linux.o

build_server:
	docker build --tag 'server' .

server: build_server
	echo "> Use ./unrealircd start to start the server"
	docker run --network=host -it 'server'