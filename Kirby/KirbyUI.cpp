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
	TypeNameUI->SetTransform({ {TypeNameUIScale.hX(), TypeNameUIScale.hY() + 450}, TypeNameUIScale });
	TypeNameUI->CameraEffectOff();
}
void KirbyUI::Tick(float _DeltaTime)
{
	TypeUIUpdate();
}

void KirbyUI::TypeUIUpdate()
{
	// Ŀ�� Ÿ���� �ٲ������ UI ������Ʈ
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