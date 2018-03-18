ENV   = LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	LIBRARY_PATH=/usr/local/lib:/usr/local/lib64
	
CC    = g++
FILES = lib/main.cpp \
	lib/Http.cpp \
	lib/Server.cpp \
	lib/Discord/Client.cpp

FLAGS = -o hideyoshi \
	-Wall \
	-pthread \
	-lssl \
	-lcurl \
	-lcurlpp \
	-lz \
	-luWS

all:
	${ENV} ${CC} ${FILES} ${FLAGS}
