#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H

#include <string>
#include "CapacitedFacilityLocationProblem/instance.h"

/**
 * @brief Abstract base class for reading instances of the Capacitated Facility Location Problem.
 */
class InstanceReader {
public:
    /**
     * @brief Virtual destructor for InstanceReader.
     */
    virtual ~InstanceReader() = default;

    /**
     * @brief Pure virtual method to read an instance from a file.
     * 
     * @param filename The name of the file containing the instance data.
     * @return Instance The read instance.
     */
    virtual Instance readInstance(const std::string& filename) const = 0;
};

#endif // INSTANCEREADER_H
