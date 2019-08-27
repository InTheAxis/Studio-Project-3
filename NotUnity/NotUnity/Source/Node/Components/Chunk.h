#ifndef CHUNK_H
#define CHUNK_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"
#include "../../Utility/Math/Spline.h"

#include <vector>

class GameObj;
class Sprite;
class Biome;
enum class INTER;
class Spline;
class KinematicBody;
class Chunk : public Node, public TypeID<Chunk>, public Component
{
public:
	Chunk(std::string name = "Chunk");
	~Chunk();

	void OnEnable();
	void OnDisable();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	Chunk* ChangeBiome();

	Spline* GetSpline();
	Sprite* GetSprite();	
	int GetCurrBiome(); //returns 0,1,2 for mt, desert, snow	
private:	
	//refs
	Sprite* sprite;
	GameObj* rock;

	//biome presets;
	Biome* biome;

	//for interactives
	void InitInteractive();
	void UpdateInteractive();
	void ConstrainTransform(Transform* t, KinematicBody* kb);
};

#endif


//#ifndef CHUNK_H
//#define CHUNK_H
//
//#include "../Node.h"
//#include "../../Utility/TypeID.h"
//#include "../../Utility/Component.h"
//#include "../../Utility/Math/Vector3.h"
//#include "../../Utility/Math/Spline.h"
//
//#include <vector>
//
//enum class BIOME_TYPE
//{
//	DESERT,
//	SNOW,
//	GRASS,
//	TOTAL_BIOME
//};
//
//enum class SPLINE_TYPE
//{
//	XYLINE,
//	MOUNTAIN,
//	TOTAL_TYPE
//};
//
//class Sprite;
//class Spline;
//class Chunk : public Node, public TypeID<Chunk>, public Component
//{
//public:
//	Chunk(std::string name = "Chunk");
//	~Chunk();
//
//	void OnEnable();
//
//	void OnDisable();
//
//	virtual void Start();
//	virtual void Update(double dt);
//	virtual void End();
//
//	void AssignBiome();
//	void AssignBiome(int i);
//	Chunk* PopulateSplineList();
//
//	void SetSpline(Spline* spline);
//	Chunk* SetSplineOffset(float offset);
//	Spline* GetSpline();
//	Sprite* GetSprite();
//private:
//	Spline* spline;
//	Sprite* sprite;
//
//	std::vector<GameObj*> GOList;
//	std::vector<Spline*> SplineList;
//
//	Vector3 HSV;
//
//	int currAnimIdx;
//
//	BIOME_TYPE currentBiome;
//
//	float splineOffsetX;
//	void GenerateMap(SPLINE_TYPE ST);
//	void UpdateMap(SPLINE_TYPE ST);
//};
//
//#endif