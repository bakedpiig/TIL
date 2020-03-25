#pragma once

template<typename tFrom,typename tTo>
class TypeAliaser {
private:
	tFrom asFromType;
	tTo asToType;
public:
	TypeAliaser(tFrom inFromValue) :
		asFromType(inFromValue) {}
	tTo& Get() { return asToType; }
};