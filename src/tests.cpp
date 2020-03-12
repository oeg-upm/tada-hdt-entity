
#include <gtest/gtest.h>
#include "entity.h"
#include "graph.h"
#include "tnode.h"


string input_file = "test.ttl";
string log_file = "test.log";
string hdt_file = "test.hdt";

string dbr_prefix = "http://dbpedia.org/resource/";
string dbo_prefix = "http://dbpedia.org/ontology/";


void ttl_to_hdt(string ttl_dir) {
    HDTSpecification spec("");
    string baseUri="<file://"+ttl_dir+">";
    string output_file;
    HDT* hdt = HDTManager::generateHDT(ttl_dir.c_str(), baseUri.c_str(), TURTLE, spec, NULL);
    output_file = ttl_dir.substr(0, ttl_dir.find_last_of(".")) +  ".hdt"; // same name but with .hdt instread of .ttl
    ofstream out;
    // Save HDT
    out.open(output_file.c_str(), ios::out | ios::binary | ios::trunc);
    if(!out.good()) {
        throw std::runtime_error("Could not open output file.");
    }
    hdt->saveToHDT(out, NULL);
    out.close();
}







namespace {


    TEST(EntityTest, Hello) {
        EntityAnn* ea = new EntityAnn(hdt_file, log_file);
        ASSERT_NE(ea, nullptr);
        delete ea;
    }

    TEST(EntityTest, GetLeafClasses) {
        EntityAnn* ea = new EntityAnn(hdt_file, log_file);
        std::list<string>* classes;
        string class_uri = dbo_prefix+"Boxer";
        string resource_uri = dbr_prefix+"boxer1";
        // boxer case easy
        classes = ea->get_leaf_classes(resource_uri);
        ASSERT_EQ(1, classes->size());
        ASSERT_STREQ(classes->front().c_str(), class_uri.c_str());
        // amature boxer case easy
        class_uri = dbo_prefix+"AmateurBoxer";
        resource_uri = dbr_prefix+"amaboxer4";
        classes = ea->get_leaf_classes(resource_uri);
        ASSERT_EQ(1, classes->size());
        ASSERT_STREQ(classes->front().c_str(), class_uri.c_str());
        // boxer case multiple class hierarchy
        class_uri = dbo_prefix+"Boxer";
        resource_uri = dbr_prefix+"boxer2";
        classes = ea->get_leaf_classes(resource_uri);
        ASSERT_EQ(1, classes->size());
        ASSERT_STREQ(classes->front().c_str(), class_uri.c_str());
        delete ea;
    }

    TEST(EntityTest, GetEntities){
        EntityAnn* ea = new EntityAnn(hdt_file, log_file);
        std::list<string>* entities;
        string label = "\"boxer1\"";
        entities = ea->get_entities_of_value(label);
        ASSERT_EQ(entities->size(), 1);
        delete ea;
    }

    TEST(EntityTest, IntermediateScoresSingleClass) {
        EntityAnn* ea = new EntityAnn(hdt_file, log_file);
        std::list<string>* classes;
        string class_uri = dbo_prefix+"Boxer";
        string resource_uri = dbr_prefix+"boxer1";
        TNode * tnode;
        string label = "\"boxer1\"";
        // entity with one class
        ea->compute_intermediate_coverage(label);
        tnode =  ea->get_tnode(class_uri);
        ASSERT_NE(tnode, nullptr);
        ASSERT_EQ(1.0, tnode->tc);
        delete ea;
    }

    TEST(EntityTest, IntermediateScoresMultiClass) {
        EntityAnn* ea = new EntityAnn(hdt_file, log_file);
        std::list<string>* classes;
        string class_uri = dbo_prefix+"Boxer";
        string resource_uri = dbr_prefix+"boxer1";
        TNode * tnode;
        string label = "\"golferboxer1\"";
        // entity with two classes
        ea->compute_intermediate_coverage(label);
        tnode =  ea->get_tnode(class_uri);
        ASSERT_NE(tnode, nullptr);
        ASSERT_DOUBLE_EQ(0.5, tnode->tc);
        delete ea;
    }

}//namespace





int main(int argc, char** argv) {
    ttl_to_hdt(input_file);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


