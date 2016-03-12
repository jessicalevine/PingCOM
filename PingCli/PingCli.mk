CPPFLAGS = -I..\PingProxyStubDll -DUNICODE -DWIN32 -D_WIN32_WINNT=0x0602 -D_WIN32_DCOM -DCOM_NO_WINDOWS_H

stdafx.o: stdafx.cpp stdafx.h
	g++ -c .\stdafx.cpp $(CPPFLAGS)

PingCli.o: PingCli.cpp stdafx.h
	g++ -c .\PingCli.cpp $(CPPFLAGS)

..\PingProxyStubDll\ping_i.o: "..\PingProxyStubDll\ping_i.c"
	gcc -c "..\PingProxyStubDll\ping_i.c" $(CPPFLAGS) -D_MIDL_USE_GUIDDEF_

# ping_p.o: "C:\Users\Administrator\Documents\Visual Studio 2015\Projects\PingCOM\PingProxyStubDll\ping_p.c"
#	gcc -c "C:\Users\Administrator\Documents\Visual Studio 2015\Projects\PingCOM\PingProxyStubDll\ping_p.c" -I../PingProxyStubDll -DUNICODE -DWIN32 -D_WIN32_WINNT=0x0602 -D_WIN32_DCOM

PingCli.exe: stdafx.o PingCli.o ..\PingProxyStubDll\ping_i.o
	g++ -o PingCli.exe ping_i.o PingCli.o stdafx.o -lole32 -luuid -loleaut32 -lkernel32 -lrpcns4 -lrpcrt4
