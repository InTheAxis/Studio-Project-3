#ifndef TEXT_H
#define TEXT_H

#include "../../Utility/TypeID.h"
#include "Renderable.h"

class MgrGraphics;
class Text : public Renderable
{
public:
	Text(std::string name = "Text") : Renderable(name) 
	{
		content = ""; 
		onScreen = true;
	}
	~Text() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	
	virtual void Render();

	Text* SetText(std::string text);
	Text* SetOnScreen(bool onScreen);

protected:
	std::string content;
	bool onScreen;
	const float textSpacing = 0.65f;
};

#endif