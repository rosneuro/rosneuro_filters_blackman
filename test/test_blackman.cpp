#include <gtest/gtest.h>
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

}

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_blackman");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}