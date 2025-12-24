#ifndef SECTIONREPOSITORY_H
#define SECTIONREPOSITORY_H

#include "Section.h"
#include <vector>

class SectionRepository {
public:
    std::vector<Section> getAllSections();
    bool addSection(const Section& section);
    bool updateSection(const Section& section);
    bool deleteSection(int id);
};

#endif // SECTIONREPOSITORY_H
