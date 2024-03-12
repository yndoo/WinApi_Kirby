#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	DoorStar,
	Monster,
	Player,
	Bullet,
	Effect,
};

enum class EKirbyCollisionOrder
{
	InhaleCol,		// ���� �浹ü
	MonsterAround,
	Monster,
	Boss,
	PlayerBullet,	// ���� �浹ü
	EdibleBullet,
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
	Brake,
	Inhale,
	Eating,
	EatingRun,
	Swallow,
	Fly,
	Attack,
	LadderUp,
	LadderDown,
	Exhale,
	Damaged,
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

// ���� :
class UContentsHelper
{
public:
	static UWindowImage* ColMapImage;
	static UWindowImage* BackgroundImage;
	static bool EatingFireMonster;			// �� �ȿ� Fire�Ӽ� ���Ͱ� �ִ� ��(���� ��Ű��X)
protected:

private:
	// constrcuter destructer
	UContentsHelper();
	~UContentsHelper();

};

