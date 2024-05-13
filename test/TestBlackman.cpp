#include <gtest/gtest.h>
#include <ros/package.h>
#include <rosneuro_filters/rosneuro_filters_utilities.hpp>
#include "Blackman.hpp"

namespace rosneuro {
    class BlackmanTestSuite : public ::testing::Test {
        public:
            BlackmanTestSuite() {}
            ~BlackmanTestSuite() {}
            void SetUp() { blackman_filter = new Blackman <double>(); }
            void TearDown() { delete blackman_filter; }
            Blackman <double>* blackman_filter;
    };

    TEST_F(BlackmanTestSuite, Constructor) {
        ASSERT_EQ(blackman_filter->name_, "blackman_filter");
        ASSERT_FALSE(blackman_filter->is_window_set_);
    }

    TEST_F(BlackmanTestSuite, Configure) {
        ASSERT_TRUE(blackman_filter->configure());
    }

    TEST_F(BlackmanTestSuite, ApplyWithValidWindow) {
        DynamicMatrix<double> inputMatrix(3, 3);
        DynamicMatrix<double> result = blackman_filter->apply(inputMatrix);

        ASSERT_EQ(result.rows(), inputMatrix.rows());
        ASSERT_EQ(result.cols(), inputMatrix.cols());

        DynamicMatrix<double> expected(3, 3);
        expected << 0,            0,            0,
                    0,  3.24249e-86, 1.47286e-319,
                    0,            0,            0;

        ASSERT_TRUE(result.isApprox(expected, 0.01));
    }

    TEST_F(BlackmanTestSuite, ApplyWithoutWindow) {
        DynamicMatrix<double> inputMatrix(-1, 0);
        ASSERT_THROW(blackman_filter->apply(inputMatrix), std::bad_alloc);
    }

    TEST_F(BlackmanTestSuite, CreateWindowWithValidSamples) {
        ASSERT_TRUE(blackman_filter->create_window(1));
    }

    TEST_F(BlackmanTestSuite, CreateWindowWithInvalidSamples) {
        ASSERT_THROW(blackman_filter->create_window(-1), std::bad_alloc);
    }

    TEST_F(BlackmanTestSuite, Integration){
        std::string base_path = ros::package::getPath("rosneuro_filters_blackman");
        const std::string input_path = base_path + "/test/rawdata.csv";
        const std::string expected_path = base_path + "/test/expected.csv";

        DynamicMatrix<double> input = readCSV<double>(input_path);
        DynamicMatrix<double> expected = readCSV<double>(expected_path);

        DynamicMatrix<double> output = blackman_filter->apply(input);
        ASSERT_TRUE(output.isApprox(expected, 1e-6));
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_blackman");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}