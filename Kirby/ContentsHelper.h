#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	DoorStar,
	Item,
	Monster,
	Player,
	Bullet,
	Effect,
	UI,
};

enum class EKirbyCollisionOrder
{
	InhaleCol,		// ���� �浹ü
	MonsterAround,
	Monster,
	IcedMonster,
	Boss,
	PlayerBullet,	// ���� �浹ü
	IceBreathBullet,
	EdibleBullet,
	LifeItem,
	IceTypeItem,
	FireTypeItem,
	Player,
	Ladder,
	Block,
	Spawner,
	Obstacle,
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
	InhaleFail,
	Changing,
};

enum class EKirbyType
{
	None,
	Normal,
	Eating,
	Fire,
	Ice,
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
	Iced,
};

enum class EBulletState
{
	None,
	Idle,
	Move,
	Damage,
	Finish,
	Inhaled, // EdibleBullet�� Inhaled�� �ʿ���
};

// ���� :
class UContentsHelper
{
public:
	static UWindowImage* ColMapImage;
	static UWindowImage* BackgroundImage;
	static EKirbyType KirbyType;
	static bool EatingFireMonster;			// �� �ȿ� Fire�Ӽ� ���Ͱ� �ִ� ��(���� ��Ű��X)
	static bool CameraRestart;				// LevelRestart�ÿ� ī�޶� ����ŸƮ �ؾ��ϴ����� ���� (true�� ī�޶� ��ġ ���� �ʿ�)
	static bool EndingLevelStart;
protected:

private:
	// constrcuter destructer
	UContentsHelper();
	~UContentsHelper();

};

