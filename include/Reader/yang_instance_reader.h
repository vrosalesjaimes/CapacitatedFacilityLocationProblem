#ifndef YANGINSTANCEREADER_H
#define YANGINSTANCEREADER_H

#include "Reader/instance_reader.h"
#include <vector>

using namespace std;

/**
 * @brief Class for reading Yang instances of the Capacitated Facility Location Problem.
 */
class YangInstanceReader : public InstanceReader {
public:
    /**
     * @brief Reads an instance from a file.
     * 
     * @param filename The name of the file containing the instance data.
     * @return Instance The read instance.
     */
    Instance readInstance(const string& filename) const override;
};

#endif // YANGINSTANCEREADER_H
