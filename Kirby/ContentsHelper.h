#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	Player,
	Monster,
	Bullet,
};

enum class EKirbyCollisionOrder
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

enum class EKirbyState
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
	Eating,
	EatingRun,
	Swallow,
	Fly,
	Attack,

};

enum class ECopyType
{
	None,
	Fire,
};

enum class EEnemyState
{
	None,
	Loading,
	Idle,
	Hurt,
	Move,
	Inhaled,
};

enum class EBulletState
{
	None,

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

