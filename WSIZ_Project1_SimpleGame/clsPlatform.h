#ifndef CLSPLATFORM_H
#define CLSPLATFORM_H
#pragma once

using namespace System;

class clsPlatform
{
public:
	Int16 posX;
	Int16 speed;

	clsPlatform();
	void move();
private:
	inline Boolean keyCheck(Int16);

};
#endif