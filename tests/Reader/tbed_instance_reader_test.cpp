#include <gtest/gtest.h>
#include "Reader/tbed_instance_reader.h"
#include "CapacitatedFacilityLocationProblem/instance.h"
#include <fstream>

// Helper function to create a test file
void createTestFileTBED(const std::string& filename, const std::string& content) {
    std::ofstream outfile(filename);
    outfile << content;
    outfile.close();
}

// Test suite for BeasleyInstanceReader
TEST(TBEDInstanceReaderTest, ReadInstanceSuccess) {
    // Create a valid test file
    std::string filename = "test_instance.txt";
    std::string content = "2 3\n\n"
                          "300 200 200\n\n"
                          "100 200\n\n"
                          "10.5 20.5\n\n"
                          "10.0 40.0 60.0\n"
                          "20.0 50.0 70.0";
    createTestFileTBED(filename, content);

    TBEDInstanceReader reader;
    Instance instance = reader.readInstance(filename);

    EXPECT_EQ(instance.getNumFacilities(), 2);
    EXPECT_EQ(instance.getNumCustomers(), 3);

    EXPECT_EQ(instance.getFacilityCapacities().size(), 2);
    EXPECT_EQ(instance.getFacilityCapacities()[0], 100);
    EXPECT_EQ(instance.getFacilityCapacities()[1], 200);

    EXPECT_EQ(instance.getOpeningCosts().size(), 2);
    EXPECT_DOUBLE_EQ(instance.getOpeningCosts()[0], 10.5);
    EXPECT_DOUBLE_EQ(instance.getOpeningCosts()[1], 20.5);

    EXPECT_EQ(instance.getCustomerDemands().size(), 3);
    EXPECT_EQ(instance.getCustomerDemands()[0], 300);
    EXPECT_EQ(instance.getCustomerDemands()[1], 200);

    EXPECT_EQ(instance.getTransportationCosts().size(), 2);
    EXPECT_EQ(instance.getTransportationCosts()[0].size(), 3);
    EXPECT_DOUBLE_EQ(instance.getTransportationCosts()[0][0], 10.0);
    EXPECT_DOUBLE_EQ(instance.getTransportationCosts()[0][1], 40.0);
    EXPECT_DOUBLE_EQ(instance.getTransportationCosts()[0][2], 60.0);

    std::remove(filename.c_str());
}

TEST(TBEDInstanceReaderTest, ReadInstanceFileNotFound) {
    TBEDInstanceReader reader;
    EXPECT_THROW(reader.readInstance("non_existent_file.txt"), std::runtime_error);
}