#ifndef BEASLEYINSTANCEREADER_H
#define BEASLEYINSTANCEREADER_H

#include "Reader/instance_reader.h"
#include <vector>

using namespace std;

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
    CFLPProblem readInstance(const string& filename) const override;
};

#endif // BEASLEYINSTANCEREADER_H
