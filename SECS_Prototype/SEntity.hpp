#pragma once

struct SEntity
{
	// Index of SEntity
	int Index;
	virtual ~SEntity() {};
};

struct SEN : public SEntity
{

};

struct SSS : public SEN {

};