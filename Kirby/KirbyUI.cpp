#include "KirbyUI.h"

KirbyUI::KirbyUI()
{
}

KirbyUI::~KirbyUI()
{
}

void KirbyUI::BeginPlay()
{
	SetActorLocation({ 0, 0 });

	TypeNameUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	TypeNameUI->SetImage("nametag_normal.png");
	FVector TypeNameUIScale = TypeNameUI->GetImage()->GetScale();
	TypeNameUI->SetTransform({ TypeNamePos, TypeNameUIScale });
	TypeNameUI->CameraEffectOff();

	KirbyHpCaseUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	KirbyHpCaseUI->SetImage("KirbyHpBarCase.png");
	FVector KirbyHpCaseUIScale = KirbyHpCaseUI->GetImage()->GetScale();
	KirbyHpCaseUI->SetTransform({ HpCasePos + KirbyHpCaseUIScale.Half2D(), KirbyHpCaseUIScale});
	KirbyHpCaseUI->CameraEffectOff();
}

void KirbyUI::Tick(float _DeltaTime)
{
	TypeUIUpdate();
}

void KirbyUI::TypeUIUpdate()
{
	// 커비 타입이 바뀌었으면 UI 업데이트
	if (TypeState != UContentsHelper::KirbyType)
	{
		TypeState = UContentsHelper::KirbyType;
		SetTypeNameUI(TypeState);
	}
}

void KirbyUI::SetTypeNameUI(EKirbyType _Type)
{
	std::string resource = "nametag";
	switch (_Type) 
	{
	case EKirbyType::Normal:
		resource += "_normal";
		break;
	case EKirbyType::Fire:
		resource += "_fire";
		break;
	default:
		break;
	}
	resource += ".png";
	TypeNameUI->SetImage(resource);
}