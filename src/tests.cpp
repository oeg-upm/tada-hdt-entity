
#include <gtest/gtest.h>
#include "entity.h"


string input_file = "sample_golf.ttl";
string log_file = "tests.log";
string hdt_file = "sample_golf.hdt";




void ttl_to_hdt(string ttl_dir){
    HDTSpecification spec("");
    string baseUri="<file://"+ttl_dir+">";
    string output_file;
    HDT *hdt = HDTManager::generateHDT(ttl_dir.c_str(), baseUri.c_str(), TURTLE, spec, NULL);
    output_file = ttl_dir.substr(0, ttl_dir.find_last_of(".")) +  ".hdt"; // same name but with .hdt instread of .ttl
    ofstream out;
    // Save HDT
    out.open(output_file.c_str(), ios::out | ios::binary | ios::trunc);
    if(!out.good()){
        throw std::runtime_error("Could not open output file.");
    }
    hdt->saveToHDT(out, NULL);
    out.close();
}







namespace {


 TEST(EntityTest, Hello){
    EntityAnn *ea = new EntityAnn(hdt_file,log_file);
        ASSERT_NE(ea,nullptr);
    }


}//namespace





int main(int argc, char **argv) {
//    ttl_to_hdt(input_file);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


