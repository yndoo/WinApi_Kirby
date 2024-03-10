#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	Monster,
	Player,
	Bullet,
};

enum class EKirbyCollisionOrder
{
	InhaleCol,		// 흡입 충돌체
	PlayerBullet,	// 공격 충돌체
	Monster,
	Player,
	Ladder,
	Block,
	Spawner,
};

enum class EActorDir
{
	None = 0,
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
	LadderUp,
	LadderDown,
	Exhale,
};

enum class ECopyType
{
	None,
	Fire,
};

enum class EEnemyState
{
	None,
	Idle,
	Damaged,
	Move,
	Inhaled,
	Die,
	HitWall,
	ShootReady,
	ShootJump,
	Shoot,
};

enum class EBulletState
{
	None,
	Idle,
	Move,
	Damage,
	Finish,
};

// 설명 :
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

