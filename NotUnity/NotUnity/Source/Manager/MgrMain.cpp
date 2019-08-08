#include "MgrMain.h"

void MgrMain::Start()
{
	//order matters

	AddChild("MgrScene", MgrScene::Instance());
	AddChild("MgrGraphics", MgrGraphics::Instance());
	
	Node::Start();
}

void MgrMain::Update(double dt)
{
	Mesh* temp = MgrGraphics::Instance()->GetCachedMesh("tri");
	MgrGraphics::Instance()->UseShader(MgrGraphics::DEFAULT);
	glBindVertexArray(temp->vao);
	glDrawArrays(temp->drawMode, 0, temp->indexSize);
	Node::Update(dt);
}

void MgrMain::End()
{
	Node::End();
}
