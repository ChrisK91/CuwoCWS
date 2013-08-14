#include <iostream>
#include <vcclr.h>
#include <Windows.h>
#using <Bridge.dll>

using namespace System;
using namespace System::Reflection;
using namespace System::IO;

ref class AssemblyResolver {
public:
	static String^ AssemblyFolder;

	Assembly^ AssemblyResolveHandler(Object^ Sender, ResolveEventArgs^ args)
	{
		AssemblyName^ name = gcnew AssemblyName(args->Name);
		
		String^ dllName = Path::Combine(AssemblyFolder, String::Format("{0}.dll", name->Name));

		if(File::Exists(dllName))
			return Assembly::LoadFile(dllName);
		else
			throw gcnew Exception(String::Format("The assembly {0} was not found", args->Name));
	}
};

class CuwoComm
{
private:
	gcroot<Bridge::Bridge^> protocolBridge;
	gcroot<String^> assemblyFolder;
public:

	CuwoComm() {
		WriteOutput("Cuwo plugin bridge loaded");

		assemblyFolder = Directory::GetCurrentDirectory();
		WriteOutput(String::Format("Current directory {0}", assemblyFolder));
		assemblyFolder = Path::Combine(assemblyFolder, "dll");

		AssemblyResolver::AssemblyFolder = assemblyFolder;

		AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(gcnew AssemblyResolver(), &AssemblyResolver::AssemblyResolveHandler);
#ifdef _DEBUG
		WriteOutput(String::Format("DLL Path {0}", assemblyFolder));
		WriteOutput("Debug build.");
		//System::Console::ReadLine();
#endif
	}

	~CuwoComm() {
		WriteOutput("Cuwo plugin bridge unloaded");
	}

	void Initialize() { 
		protocolBridge = gcnew Bridge::Bridge();
	}

	void WriteOutput(String^ text)
	{
		System::ConsoleColor bOld = Console::BackgroundColor;
		System::ConsoleColor fOld = Console::ForegroundColor;

		Console::ForegroundColor = System::ConsoleColor::White;
		Console::BackgroundColor = System::ConsoleColor::Blue;

		Console::WriteLine(text);

		Console::ForegroundColor = fOld;
		Console::BackgroundColor = bOld;
	}

	void PlayerJoined(char* name, char* ip)
	{
		String^ joinedName = gcnew String(name);
		String^ joinedIP = gcnew String(ip);

		protocolBridge->PlayerJoined(joinedName, joinedIP);
	}

	void PlayerLeft(char* ip)
	{
		String^ joinedIP = gcnew String(ip);

		protocolBridge->PlayerDisconnected(joinedIP);
	}
};

extern "C" __declspec(dllexport) CuwoComm* CuwoComm_new() { 
	return new CuwoComm(); 
}

extern "C" __declspec(dllexport) void CuwoComm_initialize(CuwoComm* obj) { 
	obj->Initialize(); 
}

extern "C" __declspec(dllexport) void CuwoComm_destruct(CuwoComm* obj) { 
	delete obj; 
}

extern "C" _declspec(dllexport) void CuwoComm_playerJoined(CuwoComm* obj, char* name, char* ip){
	obj->PlayerJoined(name, ip);
}

extern "C" _declspec(dllexport) void CuwoComm_playerLeft(CuwoComm* obj, char* ip){
	obj->PlayerLeft(ip);
}