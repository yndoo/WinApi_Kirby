#pragma once
#include <EngineCore/Level.h>
#include "Player.h"
#include "Flamer.h"
#include "Map.h"
#include "WaddleDee.h"
#include "KirbyUI.h"

// ���� :
class UPlayLevel : public ULevel
{
public:
	// constrcuter destructer
	UPlayLevel();
	~UPlayLevel();

	// delete Function
	UPlayLevel(const UPlayLevel& _Other) = delete;
	UPlayLevel(UPlayLevel&& _Other) noexcept = delete;
	UPlayLevel& operator=(const UPlayLevel& _Other) = delete;
	UPlayLevel& operator=(UPlayLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private:
	void CuttingImgLR(std::string_view _Name, int _X, int _Y);	// CuttingImage�Լ� ����� �� ���� ������ ���ÿ� ������ִ� ���Ǹ��� �Լ�

	AMap* Map = nullptr;
	KirbyUI* UI = nullptr;

	int KirbyHpData = 0;
	int KirbyLifeData = 0;
	
	AFlamer* Flamers[2] = {};
	int FlamerHpData[2] = {};
	
	AWaddleDee* WDees[2] = {};
	int WDeeHpData[2] = {};
};
