# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

MESSAGE-SERVER=		server-main.o inet-server.o server.o
MESSAGE-CLIENT=		client-main.o inet-client.o client.o
OBJS =			$(MESSAGE-SERVER) $(MESSAGE-CLIENT)

LIBS=

CCFLAGS= -g -pthread

all:	server-main client-main

server-main:$(MESSAGE-SERVER)
	$(CXX) -o server $(MESSAGE-SERVER) $(LIBS)

client-main:$(MESSAGE-CLIENT)
	$(CXX) -o client $(MESSAGE-CLIENT) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) server client


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
