#ifndef MeshImporter_h
#define MeshImporter_h
#include "Importer.h"
class MeshImporter : public Importer {
	MeshImporter() {}

	bool Import(const char*, const char*, std::string*);
	bool Import(const void*, unsigned, std::string*);
};
#endif
