#include "DebugText.h"
#include "../../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"

void DebugText::Start()
{
	fps = parent->GetParent()->GetChild<Text>();
	parent->GetParent()->GetChild <Transform>()->translate.Set(0,3,0);	
	fps->SetSize(0.5f);
}

void DebugText::Update(double dt) 
{
	ss.clear(); ss.str(""); ss.precision(3);
	ss << "Pos:" << MgrGameObj::Instance()->GetGameObj("mainCam", (Scene*)parent->GetParent()->GetParent()->GetParent())->GetTransform()->translate;
	ss << " | FPS:" << 1.0 / (dt);
	fps->SetText(ss.str());
	Node::Update(dt);
}

void DebugText::End()
{
	Node::End();
}