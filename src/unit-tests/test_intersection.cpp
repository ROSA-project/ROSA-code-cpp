#include "gtest/gtest.h"
#include "intersection_instance.hpp"
#include "object_registry.hpp"
#include "object.hpp"
#include "sphere.hpp"

double thresholdFraction = 0.015;

TEST(IntrsctionTest, NoIntersection) {

    auto registry = std::make_shared<rosa::ObjectRegistry>();
    std::weak_ptr<rosa::Object> owner_object; // nullptr

    rosa::ObjectId objID1 = registry->getNextAvailableId();
    rosa::ObjectId objID2 = registry->getNextAvailableId();
    rosa::ObjectId objID3 = registry->getNextAvailableId();

    auto sphere1 = std::make_unique<rosa::Sphere>(5.0);
    auto sphere2 = std::make_unique<rosa::Sphere>(3.0);

    rosa::Position p1{1, 4, 9, 0, 0, 0, 0};
    rosa::Position p2{20, 15, 25, 0, 0, 0, 0};

    auto obj1 = std::make_shared<rosa::Object>(
        objID1,
        "TestObject1",
        std::move(sphere1),
        p1,
        owner_object,
        registry
    );

    auto obj2 = std::make_shared<rosa::Object>(
        objID2,
        "TestObject2",
        std::move(sphere2),
        p2,
        owner_object,
        registry
    );

    auto obj3 = std::make_shared<rosa::Object>(
        objID3,
        "TestObject3",
        std::move(sphere1),
        p1,
        owner_object,
        registry
    );

    // Add the object to the registry
    registry->getObjects().emplace(objID1, obj1);
    registry->getObjects().emplace(objID2, obj2);
    registry->getObjects().emplace(objID3, obj3);



    rosa::IntersectionInstance intersectionTest(*obj1, *obj2);
    EXPECT_DOUBLE_EQ(intersectionTest.intersect(), 0.0);

    obj1->updatePosition(rosa::Position(1, 4, 9, 0, 0, 0, 0));
    obj2->updatePosition(rosa::Position(7, 0, 7, 0, 0, 0, 0));
    EXPECT_DOUBLE_EQ(intersectionTest.intersect(), 0.0);

    obj1->updatePosition(rosa::Position(0, 0, 0, 0, 0, 0, 0));
    obj2 = obj3;
    EXPECT_DOUBLE_EQ(intersectionTest.intersect(), 0.0);
}

TEST(IntersectionTest, PartialIntersectionTest) {
    auto registry = std::make_shared<rosa::ObjectRegistry>();
    std::weak_ptr<rosa::Object> owner_object; // nullptr

    rosa::ObjectId objID1 = registry->getNextAvailableId();
    rosa::ObjectId objID2 = registry->getNextAvailableId();
    rosa::ObjectId objID3 = registry->getNextAvailableId();

    auto sphere1 = std::make_unique<rosa::Sphere>(5.0);
    auto sphere2 = std::make_unique<rosa::Sphere>(2.0);
    auto sphere3 = std::make_unique<rosa::Sphere>(3.0);

    rosa::Position p1{1, 4, 9, 0, 0, 0, 0};
    rosa::Position p2{2, 0, 7, 0, 0, 0, 0};
    rosa::Position p3{6, 0, 7, 0, 0, 0, 0};

    auto obj1 = std::make_shared<rosa::Object>(
        objID1,
        "TestObject1",
        std::move(sphere1),
        p1,
        owner_object,
        registry
    );

    auto obj2 = std::make_shared<rosa::Object>(
        objID2,
        "TestObject2",
        std::move(sphere2),
        p2,
        owner_object,
        registry
    );

    auto obj3 = std::make_shared<rosa::Object>(
        objID3,
        "TestObject3",
        std::move(sphere3),
        p3,
        owner_object,
        registry
    );

    rosa::IntersectionInstance test1(*obj1, *obj2);
    rosa::IntersectionInstance test2(*obj1, *obj3);

    EXPECT_DOUBLE_EQ(test1.intersect(), 19.415981738039619);
    EXPECT_DOUBLE_EQ(test2.intersect(), 9.1391194329659378);
}

TEST(IntrsctionTest, OuterTangency) {

    auto registry = std::make_shared<rosa::ObjectRegistry>();
    std::weak_ptr<rosa::Object> owner_object; // nullptr

    rosa::ObjectId objID1 = registry->getNextAvailableId();
    rosa::ObjectId objID2 = registry->getNextAvailableId();


    auto sphere1 = std::make_unique<rosa::Sphere>(5.0);


    rosa::Position p1{1, 4, 9, 0, 0, 0, 0};
    rosa::Position p2{11, 4, 9, 0, 0, 0, 0};

    auto obj1 = std::make_shared<rosa::Object>(
        objID1,
        "TestObject1",
        std::move(sphere1),
        p1,
        owner_object,
        registry
    );

    auto obj2 = std::make_shared<rosa::Object>(
        objID2,
        "TestObject2",
        std::move(sphere1),
        p2,
        owner_object,
        registry
    );

    rosa::IntersectionInstance intersectionTest(*obj1, *obj2);

    EXPECT_DOUBLE_EQ(intersectionTest.intersect(), 0.0);
}

TEST(IntrsctionTest, InnerTangency) {

    auto registry = std::make_shared<rosa::ObjectRegistry>();
    std::weak_ptr<rosa::Object> owner_object; // nullptr

    rosa::ObjectId objID1 = registry->getNextAvailableId();
    rosa::ObjectId objID2 = registry->getNextAvailableId();


    auto sphere1 = std::make_unique<rosa::Sphere>(1.5);
    auto sphere2 = std::make_unique<rosa::Sphere>(1.0);


    rosa::Position p1{0, 0, 0, 0, 0, 0, 0};
    rosa::Position p2{-0.5, 0 , 0, 0, 0, 0, 0};

    auto obj1 = std::make_shared<rosa::Object>(
        objID1,
        "TestObject1",
        std::move(sphere1),
        p1,
        owner_object,
        registry
    );

    auto obj2 = std::make_shared<rosa::Object>(
        objID2,
        "TestObject2",
        std::move(sphere1),
        p2,
        owner_object,
        registry
    );

    rosa::IntersectionInstance intersectionTest(*obj1, *obj2);

    EXPECT_DOUBLE_EQ(intersectionTest.intersect(), 4.1887866666666662);
}

TEST(IntrsctionTest, oneSphereInsideAnother) {

    auto registry = std::make_shared<rosa::ObjectRegistry>();
    std::weak_ptr<rosa::Object> owner_object; // nullptr

    rosa::ObjectId objID1 = registry->getNextAvailableId();
    rosa::ObjectId objID2 = registry->getNextAvailableId();


    auto sphere1 = std::make_unique<rosa::Sphere>(8.0);
    auto sphere2 = std::make_unique<rosa::Sphere>(2.0);


    rosa::Position p1{-10.5, -10.5, -10.5, 0, 0, 0, 0};
    rosa::Position p2{-11.0, -11.0, -11.0, 0, 0, 0, 0};

    auto obj1 = std::make_shared<rosa::Object>(
        objID1,
        "TestObject1",
        std::move(sphere1),
        p1,
        owner_object,
        registry
    );

    auto obj2 = std::make_shared<rosa::Object>(
        objID2,
        "TestObject2",
        std::move(sphere1),
        p2,
        owner_object,
        registry
    );

    rosa::IntersectionInstance intersectionTest(*obj1, *obj2);

    EXPECT_DOUBLE_EQ(intersectionTest.intersect(), 33.51029333333333);
}