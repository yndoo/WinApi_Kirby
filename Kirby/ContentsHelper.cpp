#include "ContentsHelper.h"

UWindowImage* UContentsHelper::ColMapImage = nullptr;
UWindowImage* UContentsHelper::BackgroundImage = nullptr;

bool UContentsHelper::EatingFireMonster = false;
bool UContentsHelper::CameraRestart = false;
EKirbyType UContentsHelper::KirbyType = EKirbyType::None;

UContentsHelper::UContentsHelper()
{
}

UContentsHelper::~UContentsHelper()
{
}


