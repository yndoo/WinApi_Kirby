#pragma once
#include <EngineCore\ImageRenderer.h>

enum class KirbyRenderOrder
{
	Map,
	Monster,
	Player
};

enum class KirbyCollisionOrder
{
	PlayerBullet,	// ���� �浹ü, ���� �浹ü
	Monster,
	Player
};

enum class EActorDir
{
	Left = -1,
	Right = 1,
};

enum class EPlayState
{
	None,
	CameraFreeMove,
	FreeMove,
	Idle,
	Move,
	Crouch,
	Slide,
	Run,
	Jump,
	Break,
	Inhale,
};

enum class EEnemyState
{
	None,
	Idle,
	Hurt,
	Move,
};

// ���� :
class UContentsHelper
{
public:
	static UWindowImage* ColMapImage;
	static UWindowImage* BackgroundImage;

protected:

private:
	// constrcuter destructer
	UContentsHelper();
	~UContentsHelper();

};

