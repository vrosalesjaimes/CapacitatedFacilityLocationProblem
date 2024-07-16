#ifndef HOLMBERGINSTANCEREADER_H
#define HOLMBERGINSTANCEREADER_H

#include "Reader/instance_reader.h"
#include <vector>

/**
 * @brief Class for reading Holmberg instances of the Capacitated Facility Location Problem.
 */
class HolmbergInstanceReader : public InstanceReader {
public:
    /**
     * @brief Reads an instance from a file.
     * 
     * @param filename The name of the file containing the instance data.
     * @return Instance The read instance.
     */
    Instance readInstance(const std::string& filename) const override;
};

#endif // HOLMBERGINSTANCEREADER_H
