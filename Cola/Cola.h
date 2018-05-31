#pragma once
#include "Render/Renderer.h"

class Obj
{
public:
	Obj();
	virtual ~Obj();
	virtual void Update() {};
protected:
	void Quit();
	Renderer* GetRenderer();
private:
	size_t mId;
};

class Cola
{
public:
	Cola();
	~Cola();
	void Drink();
};
