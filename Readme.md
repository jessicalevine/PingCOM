## Steps to reproduce project structure in Visual Studio 2015

1. Create a solution with three projects
 1. Client: Win32 Cli project
 2. Proxy/stub DLL: Win32 project, select DLL radio button, and check "Empty project" in last menu
 3. DLL Server: Win32 project, select DLL radio button
 4. Exe Server: Empty project
2. Follow [this tutorial](http://blogs.msdn.com/b/eldar/archive/2006/02/28/540981.aspx) to set up the empty DLL project as the proxy/stub DLL with MIDL
3. Add the MIDL `x_h.h` file as an Existing Item to the client and server projects
4. Add the proxy/stub DLL folder to the "Additional Include Directories" under Project > Properties > C/C++ > General for the client and server projects

## Additional hints
1. Don't forget to include `<windows.h>` and `<initguid.h>` in your client. They aren't included by default but are needed to compile COM code.
2. When completing the above tutorial you may/will probably have to change the code for the `_WIN32_WINNT` to match your Windows version
3. The above instructions do not provide scaffold classes and methods necessary to implement COM or a DLL, but you can find them in the commit history
4. This repo reuses code from the DLL server for the EXE server. You may wish to refactor the generic code into a fifth project or library.
