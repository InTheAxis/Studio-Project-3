#include "DebugText.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"

DebugText::DebugText(std::string name) : Node(name) 
{
}

DebugText::~DebugText()
{
}

void DebugText::Start()
{	
	//gameObject->AddComp<Text>(); 
	//OR
	fps = AddChild<Text>();	
	fps->SetGameObj(gameObject);

	gameObject->GetComp<Transform>()->translate.Set(0,3,0);
	fps->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("text"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("font"));
	fps->SetSize(0.5f);
	Node::Start();
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