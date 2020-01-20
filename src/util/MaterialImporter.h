#ifndef MaterialImporter_h
#define MaterialImporter_h
#include "Importer.h"

struct Texture;
class MaterialImporter : public Importer {
public:
	MaterialImporter();
	~MaterialImporter();
	bool Import(const char*, const char*, std::string*);
	bool Load(const char*, Texture*);
private:
	unsigned imageName;
};
#endif
