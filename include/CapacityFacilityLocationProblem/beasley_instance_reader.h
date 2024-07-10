#ifndef BEASLEYINSTANCEREADER_H
#define BEASLEYINSTANCEREADER_H

#include "CapacityFacilityLocationProblem/instance_reader.h"
#include <vector>

/**
 * @brief Class for reading Beasley instances of the Capacitated Facility Location Problem.
 */
class BeasleyInstanceReader : public InstanceReader {
public:
    /**
     * @brief Reads an instance from a file.
     * 
     * @param filename The name of the file containing the instance data.
     * @return Instance The read instance.
     */
    Instance readInstance(const std::string& filename) const override;
};

#endif // BEASLEYINSTANCEREADER_H
