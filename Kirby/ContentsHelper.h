#pragma once
#include <EngineCore\ImageRenderer.h>

enum KirbyRenderOrder
{
	Map,
	Monster,
	Player
};

// ���� :
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

