#pragma once

class ObjectPoolable
{
	virtual bool IsNeedToReturn() = 0;
	virtual void InitForObjectPool() = 0;
};