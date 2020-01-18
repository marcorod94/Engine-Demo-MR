#ifndef Importer_h
#define Importer_h
#include <string>
class Importer {
public:
	Importer() {}

	virtual bool Import(const char*, const char*, std::string*) { return true; }
};
#endif