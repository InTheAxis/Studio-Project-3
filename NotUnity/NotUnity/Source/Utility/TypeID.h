#ifndef TYPE_ID_H
#define TYPE_ID_H

//basically this gives each derived type a unique id, inherit from TypeID<T> to use
/********************************************
  EXAMPLE USAGE

class MyClass : public TypeID<MyClass>
********************************************/

typedef unsigned long long ID;

class IDBase
{
protected:
	inline static ID GetID() { static ID m_incrementingID = 0; return m_incrementingID++; }
};

template<typename T>
class TypeID : public IDBase
{
public:
	static const ID m_id;	
};

template<typename T>
const ID TypeID<T>::m_id(IDBase::GetID());

#endif