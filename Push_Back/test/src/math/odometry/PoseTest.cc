#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <math/odometry/Pose.hpp>

/** Used to convert valarray to vector for gtest's matchers to work. */
#define POSE2VEC(pose) (std::vector(std::begin(pose.position), std::end(pose.position)))

/** Used to shorten the name of EuclideanPose in the test. */
#define EP EuclideanPose

class PoseTest : public testing::Test {
protected:
    EP e_pose{3, 4};
    EP long_pose{1, 2, 3, 4, 5};

    Pose pose{5, 12, M_PI_4};
    PathPose path_pose{20, 99, 12.5};
};

TEST_F(PoseTest, distance) {
    EXPECT_DOUBLE_EQ(e_pose.distance({6, 8}), 5);
    EXPECT_EQ(long_pose.distance({1, 2, 3, 4, 5}), 0);
    EXPECT_DOUBLE_EQ(pose.distance({5, 7}), 5);
    EXPECT_DOUBLE_EQ(path_pose.distance({23, 103}), 5);
}

TEST_F(PoseTest, addition) {
    EP result = e_pose + EP{1, 2};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(4, 6));

    result = long_pose + EP{0, 0, 0, 0, 0};
    EXPECT_THAT(POSE2VEC(result),  testing::ElementsAre(1, 2, 3, 4, 5));

    result = pose + Pose{1, 1, M_PI_4};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(6, 13));

    result = path_pose + PathPose{1, 1, 0};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(21, 100));
}

TEST_F(PoseTest, subtraction) {
    EP result = e_pose - EP{1, 2};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(2, 2));

    result = long_pose - EP{1, 1, 1, 1, 1};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(0, 1, 2, 3, 4));

    result = pose - Pose{2, 2, 0};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(3, 10));

    result = path_pose - PathPose{5, 9, 0};
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(15, 90));
}

TEST_F(PoseTest, dot_product) {
    const EP e_other{2, 1};
    const EP long_other{1, 2, 3, 4, 5};
    const Pose pose_other{2, 3, 0};
    const PathPose path_other{1, 2, 0};

    EXPECT_DOUBLE_EQ(e_pose * e_other, 10);
    EXPECT_DOUBLE_EQ(long_pose * long_other, 55);
    EXPECT_DOUBLE_EQ(pose * pose_other, 46);
    EXPECT_DOUBLE_EQ(path_pose * path_other, 218);
}

TEST_F(PoseTest, scalar_multiplication) {
    EP result = e_pose * 2.0;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(6, 8));

    result = long_pose * 0.5;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(0.5, 1, 1.5, 2, 2.5));

    result = pose * 2.0;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(10, 24));

    result = path_pose * 0.1;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(2, 9.9));
}

TEST_F(PoseTest, scalar_division) {
    EP result = e_pose / 2.0;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(1.5, 2));

    result = long_pose / 2.0;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(0.5, 1, 1.5, 2, 2.5));

    result = pose / 5.0;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(1, 2.4));

    result = path_pose / 10.0;
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(2, 9.9));
}

TEST_F(PoseTest, lerp) {
    EP result = e_pose.lerp(EP{9, 14}, 0.5);
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(6, 9));

    result = long_pose.lerp(EP{6, 7, 8, 9, 10}, 0.5);
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(3.5, 4.5, 5.5, 6.5, 7.5));

    result = pose.lerp(Pose{9, 20, 0}, 0.25);
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(6, 14));

    result = path_pose.lerp(PathPose{24, 79, 0}, 0.25);
    EXPECT_THAT(POSE2VEC(result), testing::ElementsAre(21, 94));
}
