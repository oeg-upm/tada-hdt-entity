
#include <gtest/gtest.h>
#include "Entity.h"

namespace {


 TEST(EntityTest, Hello){
	EntityAnn *ea = new EntityAnn("somehdt","somelog.log");
        ASSERT_NE(ea,nullptr);
    }


}//namespace





int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


