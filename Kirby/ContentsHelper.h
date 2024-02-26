#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	Player,
	Monster,
};

enum class EKirbyCollisionOrder
{
	PlayerBullet,	// 흡입 충돌체, 공격 충돌체
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
	Eating,
	EatingRun,
};

enum class EEnemyState
{
	None,
	Idle,
	Hurt,
	Move,
	Inhaled,
};

enum class ECopyType
{
	None,
	Fire,
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

