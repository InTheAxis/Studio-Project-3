#ifndef TEXT_H
#define TEXT_H

#include "../../Utility/Math/Vector4.h"
#include "Renderable.h"

class MgrGraphics;
class Text : public Renderable
{
public:
	Text(std::string name = "Text");
	~Text();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	
	virtual void Render();

	Text* SetText(std::string text);
	Text* SetColor(Vector4 col);
	Text* SetSize(float size);
	Text* SetOnScreen(bool onScreen);
	Text* SetAlignment(int align); //-1,0,1 = l,center,r

protected:
	std::string content;
	Vector4 color;
	bool onScreen;
	float fontSize;
	int align;
	const float textSpacing = 0.65f;
};

#endif