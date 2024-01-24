#pragma once
#include <EngineCore/EngineCore.h>

// Ό³Έν :
class KirbyCore : public EngineCore
{
public:
	// constrcuter destructer
	KirbyCore();
	~KirbyCore();

	// delete Function
	KirbyCore(const KirbyCore& _Other) = delete;
	KirbyCore(KirbyCore&& _Other) noexcept = delete;
	KirbyCore& operator=(const KirbyCore& _Other) = delete;
	KirbyCore& operator=(KirbyCore&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update() override;
	void End() override;

private:

};

