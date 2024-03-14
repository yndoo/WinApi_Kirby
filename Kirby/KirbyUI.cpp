#include "KirbyUI.h"
#include "Player.h"

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
	KirbyHpCaseUI->SetTransform({ HpCasePos + KirbyHpCaseUIScale.Half2D(), KirbyHpCaseUIScale });
	KirbyHpCaseUI->CameraEffectOff();

	KirbyHpUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	KirbyHpUI->SetImage("KirbyHp.png");
	FVector KirbyHpUIScale = KirbyHpUI->GetImage()->GetScale();
	KirbyHpUI->SetTransform({ HpCasePos + KirbyHpCaseUIScale.Half2D(), KirbyHpUIScale });
	KirbyHpUI->CameraEffectOff();
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

void KirbyUI::SetKirbyHpUI(int _CurHp)
{
	FVector CurHpScale = KirbyHpUI->GetTransform().GetScale();
	FVector CurHpPos = KirbyHpUI->GetPosition();
	float NewHpScaleX = (183.f / Kirby->GetMaxHp()) * _CurHp;	// 183은 Hp리소스 가로 scale
	float NewHpPosX = (CurHpScale.X - NewHpScaleX) / 2.f;

	KirbyHpUI->SetScale({ NewHpScaleX, CurHpScale.Y });
	KirbyHpUI->SetPosition({ CurHpPos.X - NewHpPosX, CurHpPos.Y });
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