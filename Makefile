# Written for GNU Make 4.1 built for x86_64-pc-msys

VPATH=ExecutablePingServer:PingCli:PingProxyStubDll:PingServer

CC=g++
# Ignores warnings for uknown pragmas and end-if labels that are used by the Visual Studio C++ compiler
CPPFLAGS=-c -Wall -Wno-unknown-pragmas -Wno-endif-labels -IPingProxyStubDll -IPingServer -DUNICODE
CFLAGS=CPPFLAGS
LDFLAGS=-lole32 -luuid -loleaut32

all: ExecutablePingServer.exe PingCli.exe

ExecutablePingServer.exe: ExecutablePingServer.o ping_i.o CoPingEngine.o CoPingEngineFactory.o
	$(CC) $^ -o $@ $(LDFLAGS)

PingCli.exe: PingCli.o PingCli/stdafx.o ping_i.o
	$(CC) $^ -o $@ $(LDFLAGS)

CoPingEngine.o: CoPingEngine.cpp CoPingEngine.h PingServer/stdafx.h
	$(CC) $(CPPFLAGS) $<

CoPingEngineFactory.o: CoPingEngineFactory.cpp CoPingEngineFactory.h PingServer/stdafx.h
	$(CC) $(CPPFLAGS) $<

ExecutablePingServer.o: ExecutablePingServer.cpp CoPingEngine.o CoPingEngineFactory.o
	$(CC) $(CPPFLAGS) $<

ping_i.o: ping_i.c ping_h.h
	gcc $(CPPFLAGS) $<

PingCli.o: PingCli.cpp PingCli/stdafx.h
	$(CC) $(CPPFLAGS) $<

PingCli/stdafx.o: PingCli/stdafx.cpp PingCli/stdafx.h
	$(CC) $(CPPFLAGS) $< -o $@