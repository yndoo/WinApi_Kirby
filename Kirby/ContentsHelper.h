#pragma once
#include <EngineCore\ImageRenderer.h>

enum KirbyRenderOrder
{
	Map,
	Monster,
	Player
};

enum class EActorDir
{
	Left,
	Right,
};

enum class EPlayState
{
	None,
	CameraFreeMove,
	FreeMove,
	Idle,
	Move,
	Jump,
};


// Ό³Έν :
class UContentsHelper
{
public:
	static UWindowImage* ColMapImage;

protected:

private:
	// constrcuter destructer
	UContentsHelper();
	~UContentsHelper();

};

