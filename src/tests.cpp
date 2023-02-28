
#include <gtest/gtest.h>
#include "entity.h"
#include "graph.h"
#include "tnode.h"
#include <tabular_parser/parser.h>

using namespace hdt;

string input_file = "test.ttl";
string log_file = "test.log";
string hdt_file = "test.hdt";
string base_dir = "";

string dbr_prefix = "http://dbpedia.org/resource/";
string dbo_prefix = "http://dbpedia.org/ontology/";


void ttl_to_hdt(string ttl_dir) {
  HDTSpecification spec("");
  string baseUri = "<file://" + ttl_dir + ">";
  string output_file;

  if (!EntityAnn::file_exists(ttl_dir.c_str())) {
    cout << ttl_dir << " is not found (not necessarily an error)\n";
    base_dir = "../";
    ttl_dir = base_dir + ttl_dir;
    hdt_file = base_dir + hdt_file;
    cout << "Trying: " << ttl_dir << endl;
  }

  HDT *hdt = HDTManager::generateHDT(ttl_dir.c_str(), baseUri.c_str(), TURTLE, spec, NULL);
  output_file = ttl_dir.substr(0, ttl_dir.find_last_of(".")) +  ".hdt"; // same name but with .hdt instread of .ttl
  ofstream out;
  // Save HDT
  out.open(output_file.c_str(), ios::out | ios::binary | ios::trunc);

  if (!out.good()) {
    throw std::runtime_error("Could not open output file.");
  }

  hdt->saveToHDT(out, NULL);
  out.close();
}


namespace {

TEST(EntityEmpty, Hello) {
  EntityAnn *ea = new EntityAnn();
  ASSERT_NE(ea, nullptr);
  delete ea;
}

TEST(EntityTest, Hello) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  ASSERT_NE(ea, nullptr);
  delete ea;
}

TEST(EntityTest, Constructor2) {
  HDT *hdt;
  hdt = HDTManager::mapIndexedHDT(hdt_file.c_str());
  EntityAnn *ea = new EntityAnn(hdt, log_file);
  ASSERT_NE(ea, nullptr);

  ea->type_uri = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
  ea->subclassof_uri = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
//  ea->label_uri = "http://www.w3.org/2000/01/rdf-schema#label";
  ea->clear_label_uri();
  ea->append_label_uri("http://www.w3.org/2000/01/rdf-schema#label");
  ea->set_alpha(0.5);
  ASSERT_EQ(0.5, ea->get_alpha());
  delete ea;
}

TEST(EntityTest, GetLeafClasses) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "boxer1";
  // boxer case easy
  classes = ea->get_leaf_classes(resource_uri);
  ASSERT_EQ(1, classes->size());
  ASSERT_STREQ(classes->front().c_str(), class_uri.c_str());
  // amature boxer case easy
  class_uri = dbo_prefix + "AmateurBoxer";
  resource_uri = dbr_prefix + "amaboxer4";
  classes = ea->get_leaf_classes(resource_uri);
  ASSERT_EQ(1, classes->size());
  ASSERT_STREQ(classes->front().c_str(), class_uri.c_str());
  // boxer case multiple class hierarchy
  class_uri = dbo_prefix + "Boxer";
  resource_uri = dbr_prefix + "boxer2";
  classes = ea->get_leaf_classes(resource_uri);
  ASSERT_EQ(1, classes->size());
  ASSERT_STREQ(classes->front().c_str(), class_uri.c_str());
  delete ea;
}

TEST(EntityTest, GetEntities) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *entities;
  string label = "boxer1";
  entities = ea->get_entities_of_value(label);
  ASSERT_EQ(entities->size(), 1);
  delete ea;
}

TEST(EntityTest, IntermediateScoresSingleClass) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "boxer1";
  TNode *tnode;
  string label = "boxer1";
  // entity with one class
  ea->compute_intermediate_coverage(label);
  tnode =  ea->get_tnode(class_uri);
  ASSERT_NE(tnode, nullptr);
  ASSERT_EQ(1.0, tnode->tc);
  delete ea;
}

TEST(EntityTest, IntermediateScoresMultiClass) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "boxer1";
  TNode *tnode;
  string label = "golferboxer1";
  // entity with two classes
  ea->compute_intermediate_coverage(label);
  //ea->get_graph()->print_nodes();
  tnode =  ea->get_tnode(class_uri);
  ASSERT_NE(tnode, nullptr);
  ASSERT_DOUBLE_EQ(0.5, tnode->tc);
  delete ea;
}

TEST(EntityTest, GraphLeaves) {
  std::list<TNode *> *leaves;
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "boxer1";
  TNode *tnode;
  string label = "boxer1";
  ea->compute_intermediate_coverage(label);
  Graph *graph = ea->get_graph();
  leaves = graph->get_leaves();
  //graph->print_nodes();
  ASSERT_EQ(leaves->size(), 1);
  ASSERT_STREQ(leaves->front()->uri.c_str(), class_uri.c_str());
  label = "golferboxer1";
  ea->compute_intermediate_coverage(label);
  graph = ea->get_graph();
  leaves = graph->get_leaves();
  //graph->print_nodes();
  ASSERT_EQ(leaves->size(), 2);
  delete ea;
}

TEST(EntityTest, GraphRoots) {
  std::list<TNode *> *roots;
  string root = dbo_prefix + "Agent";
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "golferboxer1";
  TNode *tnode;
  string label = "golferboxer1";
  ea->compute_intermediate_coverage(label);
  Graph *graph = ea->get_graph();
  roots = graph->get_candidate_roots();
  //graph->print_nodes();
  ASSERT_EQ(roots->size(), 1);
  ASSERT_STREQ(roots->front()->uri.c_str(), root.c_str());
  delete ea;
}

TEST(EntityTest, GraphContruction) {
  std::list<TNode *> *leaves;
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string parent_uri = dbo_prefix + "Athlete";
  string grand_uri = dbo_prefix + "Person";
  string resource_uri = dbr_prefix + "boxer1";
  TNode *tnode, *parent, *grand;
  string label = "golferboxer1";
  ea->compute_intermediate_coverage(label);
  Graph *graph = ea->get_graph();
  leaves = graph->get_leaves();

  for (auto it = leaves->cbegin(); it != leaves->cend(); it++) {
    cout << "leaves: " << (*it)->uri << endl;
  }

  //graph->print_nodes();
  ASSERT_EQ(leaves->size(), 2);
  tnode = graph->get_node(class_uri);
  ASSERT_EQ(tnode->parents->size(), 1);
  parent = tnode->parents->cbegin()->second;
  grand = parent->parents->cbegin()->second;
  ASSERT_STREQ(parent->uri.c_str(), parent_uri.c_str());
  ASSERT_STREQ(grand->uri.c_str(), grand_uri.c_str());
  delete ea;
}


TEST(EntityTest, IcLcSingle) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "boxer1";
  TNode *tnode;
  string label = "boxer1";
  // entity with one class
  ea->compute_intermediate_coverage(label);
  ea->compute_Ic_for_all();
  ea->compute_Lc_for_all();
  //ea->get_graph()->print_nodes();
  tnode =  ea->get_tnode(class_uri);
  ASSERT_NE(tnode, nullptr);
  ASSERT_EQ(1.0, tnode->lc);
  delete ea;
}

TEST(EntityTest, IcLcMulti) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *classes;
  string parent_uri = dbo_prefix + "Athlete";
  string grand_uri = dbo_prefix + "Person";
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "golferboxer1";
  TNode *tnode;
  string label = "golferboxer1";
  // entity with one class
  ea->compute_intermediate_coverage(label);
  ea->compute_Ic_for_all();
  ea->compute_Lc_for_all();
  //ea->get_graph()->print_nodes();
  tnode =  ea->get_tnode(class_uri);
  //cout << "class ic: " << tnode->ic << "Lc: " << tnode->lc << endl;
  ASSERT_NE(tnode, nullptr);
  ASSERT_DOUBLE_EQ(0.5, tnode->lc);
  tnode =  ea->get_tnode(parent_uri);
  //cout << "parent ic: " << tnode->ic << "Lc: " << tnode->lc << endl;
  ASSERT_DOUBLE_EQ(1.0, tnode->lc);
  tnode =  ea->get_tnode(grand_uri);
  //cout << "grand ic: " << tnode->ic << "Lc: " << tnode->lc << endl;
  ASSERT_DOUBLE_EQ(1.0, tnode->lc);
  delete ea;
}

TEST(EntityTest, ClassEntityCounts) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  string parent_uri = dbo_prefix + "Athlete";
  string grand_uri = dbo_prefix + "Person";
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "golferboxer1";
  TNode *boxer_tnode, *amature_tnode, *agent_tnode, *person_tnode;
  string label = "golferboxer1";
  label = "amaboxer4";
  ea->compute_intermediate_coverage(label);
  ea->compute_Ic_for_all();
  ea->compute_Lc_for_all();
  //ea->get_graph()->print_nodes();
  ea->get_graph()->pick_root();
  ASSERT_STREQ((dbo_prefix + "Agent").c_str(), ea->get_graph()->get_root()->uri.c_str());
  ea->compute_classes_entities_counts();
  ea->compute_Is_for_all();
  ea->compute_Ls_for_all();
  //ea->get_graph()->print_nodes();
  boxer_tnode = ea->get_graph()->get_node(dbo_prefix + "Boxer");
  amature_tnode = ea->get_graph()->get_node(dbo_prefix + "AmateurBoxer");
  agent_tnode = ea->get_graph()->get_node(dbo_prefix + "Agent");
  person_tnode = ea->get_graph()->get_node(dbo_prefix + "Person");
  ASSERT_LT(boxer_tnode->ls, person_tnode->ls);
  ASSERT_LT(boxer_tnode->ls, agent_tnode->ls);
  ASSERT_GT(boxer_tnode->is, amature_tnode->is);
  ASSERT_GT(boxer_tnode->ls, amature_tnode->ls);
  delete ea;
}

TEST(EntityTest, NONExistantLabel) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  string parent_uri = dbo_prefix + "Athlete";
  string grand_uri = dbo_prefix + "Person";
  string class_uri = dbo_prefix + "Boxer";
  string resource_uri = dbr_prefix + "golferboxer1";
  string label = "nonexsitant";
  ea->compute_intermediate_coverage(label);
  ea->compute_Ic_for_all();
  ea->compute_Lc_for_all();
  //ea->get_graph()->print_nodes();
  ea->get_graph()->pick_root();
  ASSERT_EQ(ea->get_graph()->get_root(), nullptr);
  ea->compute_classes_entities_counts();
  ea->compute_Is_for_all();
  ea->compute_Ls_for_all();
  //ea->get_graph()->print_nodes();
  delete ea;
}

TEST(EntityTest, Scores) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *candidates;
  string class_uri = dbo_prefix + "Boxer";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test1.csv");
  data = p.parse();
  candidates = ea->annotate_column(data, 2, 0.1);
  //ea->get_graph()->print_nodes();
  ASSERT_STREQ(class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, ScoresSample) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *candidates;
  string class_uri = dbo_prefix + "Boxer";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test1.csv");
  data = p.parse();
  ea->set_sample_size(1);
  candidates = ea->annotate_column(data, 2, 0.1);
  //ea->get_graph()->print_nodes();
  ASSERT_EQ(ea->get_m(), 1);
  ASSERT_STREQ(class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, ScoresExtraRoot) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *candidates;
  string class_uri = dbo_prefix + "Swimmer";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test2.csv");
  data = p.parse();
  candidates = ea->annotate_column(data, 2, 0.1);
  //ea->get_graph()->print_nodes();
  ASSERT_STREQ(class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, Context) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.1);
  //EntityAnn* ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *candidates;
  string volley_class_uri = dbo_prefix + "VolleyballPlayer";
  string football_class_uri = dbo_prefix + "FootballPlayer";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test3.csv");
  data = p.parse();
  candidates = ea->annotate_column(data, 1, 0.1);
  ASSERT_STREQ(volley_class_uri.c_str(), candidates->front().c_str());
  delete ea;
  delete data;
  ea = new EntityAnn(hdt_file, log_file, 0.1);
  data = p.parse_vertical();
  candidates = ea->annotate_column(data, 1, true, false);
//  ea->get_graph()->print_nodes();
  ASSERT_STREQ(football_class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, ContextSample) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.1);
  std::list<string> *candidates;
  string volley_class_uri = dbo_prefix + "VolleyballPlayer";
  string football_class_uri = dbo_prefix + "FootballPlayer";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test3.csv");
  data = p.parse();
  candidates = ea->annotate_column(data, 1, 0.1);
  ASSERT_STREQ(volley_class_uri.c_str(), candidates->front().c_str());
  ASSERT_EQ(ea->get_m(), 3);
  delete ea;

  ea = new EntityAnn(hdt_file, log_file, 0.1);
  ea->set_sample_size(2);
  candidates = ea->annotate_column(data, 1, 0.1);
  ASSERT_STREQ(volley_class_uri.c_str(), candidates->front().c_str());
  ASSERT_EQ(ea->get_m(), 2);
  delete ea;

}

TEST(EntityTest, DoubleLevel) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.1);
  //EntityAnn* ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *candidates;
  string wrestler_class_uri = dbo_prefix + "Wrestler";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test4.csv");
  data = p.parse_vertical();
  candidates = ea->annotate_column(data, 1, true, true);
  //ea->get_graph()->print_nodes();
  ASSERT_STREQ(wrestler_class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, recomputef) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0);
  //EntityAnn* ea = new EntityAnn(hdt_file, log_file);
  std::list<string> *candidates;
  string wrestler_class_uri = dbo_prefix + "Wrestler";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test4.csv");
  data = p.parse_vertical();
  candidates = ea->annotate_column(data, 1, true, true);
  //ea->get_graph()->print_nodes();
  delete candidates;
  candidates = ea->recompute_f(0.1);
  ASSERT_STREQ(wrestler_class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, LangTag) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0);
  ea->set_language_tag("@en");
  std::list<string> *candidates;
  string class_uri = dbo_prefix + "Cyclist";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test5.csv");
  data = p.parse_vertical();
  candidates = ea->annotate_column(data, 0, true, true);
  //ea->get_graph()->print_nodes();
  delete candidates;
  candidates = ea->recompute_f(0.1);
  ASSERT_STREQ(class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, TitleCaseSimple) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0);
  ea->set_language_tag("@en");
  ASSERT_STRCASEEQ(ea->get_title_case("ABC").c_str(), "Abc");
  ASSERT_STRCASEEQ(ea->get_title_case("Title Case Already").c_str(), "Title Case Already");
  ASSERT_STRCASEEQ(ea->get_title_case("Another Title").c_str(), "Another Title");
  ASSERT_STRCASEEQ(ea->get_title_case("everything here is small").c_str(), "Everything Here Is Small");
  ASSERT_STRCASEEQ(ea->get_title_case("What about this?").c_str(), "What About This?");
  delete ea;
}

TEST(EntityTest, TitleCase) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0);
  ea->set_language_tag("@en");
  ea->set_title_case(true);
  std::list<string> *candidates;
  string class_uri = dbo_prefix + "BaseballPlayer";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test6.csv");
  data = p.parse_vertical();
  candidates = ea->annotate_column(data, 0, true, true);
  //ea->get_graph()->print_nodes();
  delete candidates;
  candidates = ea->recompute_f(0.1);
  ASSERT_STREQ(class_uri.c_str(), candidates->front().c_str());
  delete ea;
}

TEST(EntityTest, StripQ) {
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0);
  ea->set_language_tag("@en");
  ea->set_title_case(true);
  ASSERT_STREQ("ABC", ea->strip_quotes("\"ABC\"").c_str());
}

TEST(EntityTest, textColumn) {
  string dbp_color = "http://dbpedia.org/property/color";
  std::list<std::list<string>*> *data;
  std::list<string> *properties;
  Parser p(base_dir + "test_files/test10.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(false);
  properties = ea->annotate_entity_property_column(data, 1, 2);
//        cout << "size: "<<properties->size()<<endl;
//        cout << "front: <"<<properties->front()<<"> "<<endl;
//        cout << "target: <"<<dbp_country<<"> "<<endl;
  ASSERT_EQ(properties->size(), 1);
  ASSERT_STREQ(properties->front().c_str(), dbp_color.c_str());
}

TEST(EntityTest, textTitlecaseColumn) {
  string dbp_color = "http://dbpedia.org/property/color";
  std::list<std::list<string>*> *data;
  std::list<string> *properties;
  Parser p(base_dir + "test_files/test12.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(true);
  properties = ea->annotate_entity_property_column(data, 1, 2);
//        cout << "size: "<<properties->size()<<endl;
//        cout << "front: <"<<properties->front()<<"> "<<endl;
//        cout << "target: <"<<dbp_country<<"> "<<endl;
  ASSERT_EQ(properties->size(), 1);
  ASSERT_STREQ(properties->front().c_str(), dbp_color.c_str());
}

TEST(EntityTest, nonSubjectColumns) {
  string dbp_country = "http://dbpedia.org/property/country";
  std::list<std::list<string>*> *data;
  std::list<string> *properties;
  Parser p(base_dir + "test_files/test7.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(false);
  properties = ea->annotate_entity_property_column(data, 1, 2);
//        cout << "size: "<<properties->size()<<endl;
//        cout << "front: <"<<properties->front()<<"> "<<endl;
//        cout << "target: <"<<dbp_country<<"> "<<endl;
  ASSERT_STREQ(properties->front().c_str(), dbp_country.c_str());
}

TEST(EntityTest, textColumnsHeuristic) {
  string dbp_color = "http://dbpedia.org/property/color";
  string class_uri = "http://dbpedia.org/ontology/Boot";
  std::list<std::list<string>*> *data;
  std::list<string> *properties;
  Parser p(base_dir + "test_files/test11.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(false);
  properties = ea->annotate_entity_property_column(data, 1, 2);
  ASSERT_EQ(0, properties->size());
  properties = ea->annotate_text_property_heuristic(data, class_uri, 2);
  ASSERT_STREQ(properties->front().c_str(), dbp_color.c_str());
}

TEST(EntityTest, textTitlecaseColumnsHeuristic) {
  string dbp_color = "http://dbpedia.org/property/color";
  string class_uri = "http://dbpedia.org/ontology/Boot";
  std::list<std::list<string>*> *data;
  std::list<string> *properties;
  Parser p(base_dir + "test_files/test13.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(true);
  properties = ea->annotate_entity_property_column(data, 1, 2);
  ASSERT_EQ(0, properties->size());
  properties = ea->annotate_text_property_heuristic(data, class_uri, 2);
  ASSERT_STREQ(properties->front().c_str(), dbp_color.c_str());
}

TEST(EntityTest, nonSubjectColumnsHeuristic) {
  string dbp_country = "http://dbpedia.org/property/country";
  string class_uri = "http://dbpedia.org/ontology/Wrestler";
  std::list<std::list<string>*> *data;
  std::list<string> *properties;
  Parser p(base_dir + "test_files/test8.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(false);
  properties = ea->annotate_entity_property_column(data, 1, 2);
  ASSERT_EQ(0, properties->size());
  properties = ea->annotate_entity_property_heuristic(data, class_uri, 2);
  ASSERT_STREQ(properties->front().c_str(), dbp_country.c_str());
}

TEST(EntityTest, diamondTaxomony) {
  std::list<string> *candidates;
  string class_uri = "http://dbpedia.org/ontology/Dup";
  string mid_uri = "http://dbpedia.org/ontology/Dmid1";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test9.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->set_title_case(false);
  candidates = ea->annotate_column(data, 0, true, true);
  ASSERT_STREQ(candidates->front().c_str(), class_uri.c_str());
  ASSERT_EQ(ea->get_counts_of_class(class_uri), 4);
  ASSERT_EQ(ea->get_counts_of_class(mid_uri), 2);
  delete candidates;
}

TEST(EntityTest, multipleLabels) {
  std::list<string> *candidates;
  string class_uri = "http://dbpedia.org/ontology/Dup";
  string mid_uri = "http://dbpedia.org/ontology/Dmid1";
  std::list<std::list<string>*> *data;
  Parser p(base_dir + "test_files/test9.csv");
  data = p.parse_vertical();
  EntityAnn *ea = new EntityAnn(hdt_file, log_file, 0.9);
  ea->clear_label_uri();
  ea->append_label_uri("http://some.example");
  ea->append_label_uri("http://www.w3.org/2000/01/rdf-schema#label");
  ea->set_title_case(true);
  candidates = ea->annotate_column(data, 0, true, true);

  ASSERT_GT(candidates->size(), 0);
  ASSERT_STREQ(candidates->front().c_str(), class_uri.c_str());
  ASSERT_EQ(ea->get_counts_of_class(class_uri), 4);
  ASSERT_EQ(ea->get_counts_of_class(mid_uri), 2);
  delete candidates;
}


//    TEST(EntityTest, Temp){
//        EntityAnn* ea = new EntityAnn("/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt", log_file, 0);
//        ea->set_language_tag("@en");
//        //EntityAnn* ea = new EntityAnn(hdt_file, log_file);
//        std::list<string>* candidates;
//        string wrestler_class_uri = dbo_prefix+"Country";
//        std::list<std::list<string>*>* data;
//        Parser p("/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/11688006_0_8123036130090004213.csv");
//        data = p.parse_vertical();
//        for(auto it=data->cbegin();it!=data->cend();it++){
//                for(auto jt=(*it)->cbegin();jt!=(*it)->cend();jt++){
//                    cout << "<" << (*jt) << ">" << "|";
//                }
//                cout << endl<<"----------"<<endl;
//        }
//        candidates = ea->annotate_column(data, 1, true, true);
//        ea->get_graph()->print_nodes();
//        delete candidates;
//        candidates = ea->recompute_f(0.1);
//        ASSERT_STREQ(wrestler_class_uri.c_str(),candidates->front().c_str());
//        delete ea;
//    }

}//namespace



int main(int argc, char **argv) {
  ttl_to_hdt(input_file);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


