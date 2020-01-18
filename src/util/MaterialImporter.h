#ifndef MaterialImporter_h
#define MaterialImporter_h
#include "Importer.h"

struct Texture {
	unsigned id;
	unsigned width;
	unsigned height;
	unsigned char* data;
	std::string name;
};

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
