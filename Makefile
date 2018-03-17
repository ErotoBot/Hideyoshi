CC = g++
FILES = lib/Http.cpp \
	lib/main.cpp \
	lib/Discord/Client.cpp

FLAGS = -o hideyoshi \
	-Wall \
	-pthread \
	-lssl \
	-lcurl \
	-lcurlpp \
	-lz \
	-luWS \
	-lboost_system

all:
	${CC} ${FILES} ${FLAGS}
