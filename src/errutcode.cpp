#include "errorbase.h"

using namespace errut;

class DummyErrutObject : public ErrorBase
{
public:
	DummyErrutObject() { }
	~DummyErrutObject() { }

	bool function();
};

bool DummyErrutObject::function()
{
	setErrorString("Bla!");
	return false;
}
