CPPFLAGS = -I..\PingProxyStubDll -I..\PingServer 

# -DUNICODE -D_WIN32_WINNT=0x0602 -DWIN32 -D_WIN32_DCOM -DCOM_NO_WINDOWS_H

ExecutablePingServer.o: ExecutablePingServer.cpp
	g++ -c .\ExecutablePingServer.cpp $(CPPFLAGS)

..\PingServer\CoPingEngine.o: "..\PingServer\CoPingEngine.cpp"
	g++ -c "..\PingServer\CoPingEngine.cpp" $(CPPFLAGS)

..\PingServer\CoPingEngineFactory.o: "..\PingServer\CoPingEngineFactory.cpp"
	g++ -c "..\PingServer\CoPingEngineFactory.cpp" $(CPPFLAGS)

..\PingProxyStubDll\ping_i.o: "..\PingProxyStubDll\ping_i.c"
	gcc -c "..\PingProxyStubDll\ping_i.c" $(CPPFLAGS)

# -D_MIDL_USE_GUIDDEF_

# ping_p.o: "C:\Users\Administrator\Documents\Visual Studio 2015\Projects\PingCOM\PingProxyStubDll\ping_p.c"
#	gcc -c "C:\Users\Administrator\Documents\Visual Studio 2015\Projects\PingCOM\PingProxyStubDll\ping_p.c" -I../PingProxyStubDll -DUNICODE -DWIN32 -D_WIN32_WINNT=0x0602 -D_WIN32_DCOM

ExecutablePingServer.exe: ExecutablePingServer.o ..\PingServer\CoPingEngine.o ..\PingServer\CoPingEngineFactory.o ..\PingProxyStubDll\ping_i.o
	g++ -o ExecutablePingServer.exe ping_i.o ExecutablePingServer.o CoPingEngine.o CoPingEngineFactory.o -lole32 -luuid -loleaut32
# -lkernel32 -lrpcns4 -lrpcrt4
