#include <Windows.h>
#include <EngineCore\EngineCore.h>
#include <EnginePlatform\EngineWindow.h>
#include <Kirby\KirbyCore.h>

// ���� �������� �����鿡�� ������ �⵿�� �����ϱ� ���ؼ� ����� ���.

// ����ڰ� �ϰ� �������� ��� ��� �����ϰ� ����ų�?
// ���� ������ ũ�⸦ �����ϰ� �;�.

//ENGINESTART(ContentsCore);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPWSTR    lpCmdLine, 
	_In_ int       nCmdShow) 
{ 
	LeakCheck; 
	KirbyCore NewUserCore = KirbyCore();
	//UEngineCore::EngineStart(hInstance, &NewUserCore);
	NewUserCore.EngineStart(hInstance);

}