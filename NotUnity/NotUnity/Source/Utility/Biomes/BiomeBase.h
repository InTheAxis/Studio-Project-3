#ifndef BIOME_H
#define BIOME_H

class GameObj;
class Spline;

enum class INTER
{
	NONE = 0,
	ROCK,
	TREE,
	BARREL,
	CRATE,
};

class Biome
{
public:

	Biome();
	virtual ~Biome();

	//getters
	int GetSpriteIdx();
	Spline* GetSpline();
	INTER GetInteractiveType();
protected:
	int spriteIdx;
	Spline* spline;
	INTER interactiveType;
};

#endif