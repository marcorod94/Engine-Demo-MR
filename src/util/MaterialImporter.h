#ifndef MaterialImporter_h
#define MaterialImporter_h
#include "Importer.h"
class MaterialImporter : public Importer {
	MaterialImporter() {}

	bool Import(const char*, const char*, std::string*);
	bool Import(const void*, unsigned, std::string*);
};
#endif
