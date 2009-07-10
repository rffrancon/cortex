#include <CUnit.h>
#include <Basic.h>
#include <dB_graph.h>
#include <element.h>
#include <binary_kmer.h>
#include <file_reader.h>
#include "test_dB_graph_population.h"
#include "dB_graph_population.h"
#include <global.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

void test_db_graph_supernode_for_specific_person_or_pop()
{

 //first set up the hash/graph
  int kmer_size = 3;
  int number_of_bits = 8;
  int bucket_size    = 8;
  long long bad_reads = 0;
  int max_retries=10;

  dBGraph * hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  int seq_loaded = load_population_as_fasta("../data/test/pop_graph/supernode/one_person_one_long_supernode_with_conflict_at_end", &bad_reads, hash_table);
  CU_ASSERT(seq_loaded==13);
  CU_ASSERT(bad_reads==0);

  //we have loaded a fasta containing these supernodes ACATT, TTC, TTG
  // reads are: ACATT, ATTC, ATTG
  //note we have loaded one person only
 

  int max_expected_supernode_length=100;
  dBNode * nodes_path[max_expected_supernode_length];
  Orientation orientations_path[max_expected_supernode_length];
  Nucleotide labels_path[max_expected_supernode_length];
  char seq[max_expected_supernode_length+hash_table->kmer_size+1];

  //get the supernodes one at a time. Remember length is the number of edges between nodes.

  dBNode* test_elem1 = hash_table_find(element_get_key(seq_to_binary_kmer("ACA", kmer_size),kmer_size), hash_table);
  dBNode* test_elem2 = hash_table_find(element_get_key(seq_to_binary_kmer("CAT", kmer_size),kmer_size), hash_table);
  dBNode* test_elem3 = hash_table_find(element_get_key(seq_to_binary_kmer("ATT", kmer_size),kmer_size), hash_table);
  CU_ASSERT(!(test_elem1 == NULL));
  CU_ASSERT(!(test_elem2 == NULL));
  CU_ASSERT(!(test_elem3 == NULL));


  CU_ASSERT(db_node_check_status(test_elem1,none));
  CU_ASSERT(db_node_check_status(test_elem2,none));
  CU_ASSERT(db_node_check_status(test_elem3,none));
  
  int length = db_graph_supernode_for_specific_person_or_pop(test_elem1,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							     seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 0); //0 because we are looking at person 0 - the only person in the graph

  CU_ASSERT(length==5);
  CU_ASSERT_STRING_EQUAL(seq, "ACATT");
  CU_ASSERT(db_node_check_status(test_elem1, visited));
  CU_ASSERT(db_node_check_status(test_elem2, visited));
  CU_ASSERT(db_node_check_status(test_elem3, visited));




  //having done this, if I try to print the supernode for CAT, it shpuld refuse (providing I put in the condition that node mut not be visited)
 
  length = db_graph_supernode_for_specific_person_or_pop(test_elem2,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							 seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 0); //0 because we are looking at person 0 - the only person in the graph


  CU_ASSERT(length==0);
  CU_ASSERT(db_node_check_status(test_elem1, visited));
  CU_ASSERT(db_node_check_status(test_elem2, visited));
  CU_ASSERT(db_node_check_status(test_elem3, visited));


  hash_table_free(&hash_table);


  // ******** try another example ******************
  

  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  seq_loaded = load_population_as_fasta("../data/test/pop_graph/test_pop_load_and_print/two_individuals_simple.txt", &bad_reads, hash_table);

  //person 1:
  //>person1_read1
  //AAAAAAAAAAAAAAA
  //>person1_read2
  //ACGTT     <<<<<<<<<<<<<<<<Note this generates supernode GACGTT due to a hairpin
  //person 2:
  //>person2_read1
  //GGGGGGGGGGGGGGGGGG
  //>person2_read2
  //TTGACG

  test_elem1 = hash_table_find(element_get_key(seq_to_binary_kmer("AAA", kmer_size),kmer_size), hash_table);
  test_elem2 = hash_table_find(element_get_key(seq_to_binary_kmer("ACG", kmer_size),kmer_size), hash_table);
  test_elem3 = hash_table_find(element_get_key(seq_to_binary_kmer("CGT", kmer_size),kmer_size), hash_table);
  dBNode* test_elem4 = hash_table_find(element_get_key(seq_to_binary_kmer("GTT", kmer_size),kmer_size), hash_table);
  dBNode* test_elem5 = hash_table_find(element_get_key(seq_to_binary_kmer("GGG", kmer_size),kmer_size), hash_table);
  dBNode* test_elem6 = hash_table_find(element_get_key(seq_to_binary_kmer("TTG", kmer_size),kmer_size), hash_table);
  dBNode* test_elem7 = hash_table_find(element_get_key(seq_to_binary_kmer("TGA", kmer_size),kmer_size), hash_table);
  dBNode* test_elem8 = hash_table_find(element_get_key(seq_to_binary_kmer("GAC", kmer_size),kmer_size), hash_table);
  dBNode* test_elem9 = hash_table_find(element_get_key(seq_to_binary_kmer("ACG", kmer_size),kmer_size), hash_table);


  CU_ASSERT(!(test_elem1 == NULL));
  CU_ASSERT(!(test_elem2 == NULL));
  CU_ASSERT(!(test_elem3 == NULL));
  CU_ASSERT(!(test_elem4 == NULL));
  CU_ASSERT(!(test_elem5 == NULL));
  CU_ASSERT(!(test_elem6 == NULL));
  CU_ASSERT(!(test_elem7 == NULL));
  CU_ASSERT(!(test_elem8 == NULL));
  CU_ASSERT(!(test_elem9 == NULL));



  length = db_graph_supernode_for_specific_person_or_pop(test_elem1,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							 seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 0); //person 0 int his array is person in person1.fasta - sorry, offset by 1


  CU_ASSERT(length==4);
  CU_ASSERT_STRING_EQUAL(seq, "AAAA");
  CU_ASSERT(db_node_check_status(test_elem1, visited)==true); 
  CU_ASSERT(db_node_check_status(test_elem2, none)==true);
  CU_ASSERT(db_node_check_status(test_elem3, none)==true);
  CU_ASSERT(db_node_check_status(test_elem4, none)==true);
  CU_ASSERT(db_node_check_status(test_elem5, none)==true);
  CU_ASSERT(db_node_check_status(test_elem6, none)==true);
  CU_ASSERT(db_node_check_status(test_elem7, none)==true);
  CU_ASSERT(db_node_check_status(test_elem8, none)==true);
  CU_ASSERT(db_node_check_status(test_elem9, none)==true);
  db_graph_set_all_visited_nodes_to_status_none(hash_table);
  CU_ASSERT(db_node_check_status(test_elem1, none)==true);

  length = db_graph_supernode_for_specific_person_or_pop(test_elem2,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							 seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 0); //person 0 int his array is person in person1.fasta - sorry, offset by 1


  CU_ASSERT(length==6);
  CU_ASSERT_STRING_EQUAL(seq, "AACGTT");
  CU_ASSERT(db_node_check_status(test_elem1, none)==true);
  CU_ASSERT(db_node_check_status(test_elem2, visited)==true);
  CU_ASSERT(db_node_check_status(test_elem3, visited)==true);
  CU_ASSERT(db_node_check_status(test_elem4, visited)==true); 
  CU_ASSERT(db_node_check_status(test_elem5, none)==true);
  CU_ASSERT(db_node_check_status(test_elem6, none)==true);
  CU_ASSERT(db_node_check_status(test_elem7, none)==true);
  CU_ASSERT(db_node_check_status(test_elem8, none)==true);
  CU_ASSERT(db_node_check_status(test_elem9, visited)==true);

  length = db_graph_supernode_for_specific_person_or_pop(test_elem3,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							 seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 0); //person 0 int his array is person in person1.fasta - sorry, offset by 1

  
  CU_ASSERT(length==0);//because was already visited
  length = db_graph_supernode_for_specific_person_or_pop(test_elem4,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							 seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 0); //person 0 int his array is person in person1.fasta - sorry, offset by 1

  
  CU_ASSERT(length==0);

  CU_ASSERT(db_node_check_status(test_elem1, none)==true);
  CU_ASSERT(db_node_check_status(test_elem2, visited)==true);
  CU_ASSERT(db_node_check_status(test_elem3, visited)==true);
  CU_ASSERT(db_node_check_status(test_elem4, visited)==true);
  CU_ASSERT(db_node_check_status(test_elem5, none)==true);
  CU_ASSERT(db_node_check_status(test_elem6, none)==true);
  CU_ASSERT(db_node_check_status(test_elem7, none)==true);
  CU_ASSERT(db_node_check_status(test_elem8, none)==true);
  CU_ASSERT(db_node_check_status(test_elem9, visited)==true);

  length = db_graph_supernode_for_specific_person_or_pop(test_elem6,max_expected_supernode_length,&db_node_check_status_not_pruned_or_visited,&db_node_action_set_status_visited_or_visited_and_exists_in_reference,
							 seq,nodes_path,orientations_path, labels_path,hash_table, individual_edge_array, 1); //person 1 int his array is person in person2.fasta - sorry, offset by 1

  
  CU_ASSERT(length==6);
  CU_ASSERT_STRING_EQUAL(seq, "CGTCAA");  


  

  hash_table_free(&hash_table);
}


void test_is_supernode_end()
{

  //first set up the hash/graph
  int kmer_size = 3;
  int number_of_bits = 8;
  int bucket_size    = 8;
  long long bad_reads = 0;
  int max_retries=10;

  dBGraph * hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);

 
  //1. Sequence of tests as follows
  //         Each test loads a single specifically designed fasta file into a graph/hash table.
  //         The test then picks an element in the graph, and calls get_seq_from_elem_to_end_of_supernode
  //         and checks that it gets the right sequence.
  

  // ****
  //1.1 Fasta file that generate a graph with two hairpins, and a single edge (in each rorientation) joining them.
  //  Sequence is :  ACGTAC
  // ****

  int seq_loaded = load_population_as_fasta("../data/test/pop_graph/supernode/one_person_two_self_loops",  &bad_reads, hash_table);
  CU_ASSERT(seq_loaded==6);
  CU_ASSERT(bad_reads==0);

  //GTA is not the end of a supernode in either direction
  dBNode* query_node = hash_table_find(element_get_key(seq_to_binary_kmer("GTA",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(!db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));

  //CGT is not the end of a supernode in either direction
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("CGT",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(!db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));

  //ACG is not  the end of a supernode in the reverse direction
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ACG",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(!db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));

  hash_table_free(&hash_table);


  // ****
  //1.2 Fasta file that generate a graph with one long supernode, with a conflict at the end
  //   caused by two outward/exiting edges 
  // ****



  //first set up the hash/graph
  kmer_size = 3;
  number_of_bits = 4;
  bucket_size    = 4;
  bad_reads = 0;
  max_retries=10;

  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/supernode/one_person_one_long_supernode_with_conflict_at_end", &bad_reads, hash_table);

  CU_ASSERT(seq_loaded==13);
  CU_ASSERT(bad_reads==0);

  //ACA IS  the end of a supernode in the reverse direction
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ACA",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));
  

  //ATG is not a supernode end
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ATG",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(!db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));


  //ATT IS  the end of a supernode in the reverse direction
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ATT",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));
  
  //TTC is a supernode end in both directions
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("TTC",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));

  //TTG is a supernode end in both directions
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("TTG",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));


  hash_table_free(&hash_table);


  // ****
  //1.3 Fasta file that generate a graph with one long supernode, with a conflict at the end
  //   caused by two INWARD edges in the opposite direction
  // ****

  //first set up the hash/graph
  kmer_size = 3;
  number_of_bits = 4;
  bucket_size    = 4;
  bad_reads = 0;
  max_retries=10;


  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);


  seq_loaded=load_population_as_fasta("../data/test/pop_graph/supernode/one_person_one_long_supernode_with_inward_conflict_at_end",&bad_reads, hash_table);

  CU_ASSERT(seq_loaded==13);
  CU_ASSERT(bad_reads==0);

  //ACA IS  the end of a supernode in the reverse direction
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ACA",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));
  
  //ATG is not a supernode end
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ATG",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(!db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));


  //ATT IS  the end of a supernode in the reverse direction
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ATT",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));
  
  //TTC is a supernode end in both directions
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("TTC",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));

  //AAT is a supernode end in reverse directions
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("AAT",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(!db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));

  //TAA is a supernode end in both directions
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("TAA",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));


  hash_table_free(&hash_table);



  // ****
  //1.4 Fasta file that generate a graph with an infinite loop at a single kmer
  //   
  // ****

  
  //first set up the hash/graph
  kmer_size = 3;
  number_of_bits = 4;
  bucket_size    = 4;
  bad_reads = 0;
  max_retries=10;


  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/supernode/one_person_infiniteloop",  &bad_reads, hash_table);
  CU_ASSERT(bad_reads==0);
  CU_ASSERT(seq_loaded==25);

  //AAA is a supernode end in both directions
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("AAA",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_is_supernode_end(query_node, forward, individual_edge_array, 0, hash_table));
  CU_ASSERT(db_node_is_supernode_end(query_node, reverse, individual_edge_array, 0, hash_table));


  hash_table_free(&hash_table);


}


void test_getting_stats_of_how_many_indivduals_share_a_node()
{ 


  //       >person1_read1
  //        AAAAAAAAAAAAAAA
  //        >person1_read2
  //        ACGTT
  //
  //
  //       >person2_read1
  //       GGGGGGGGGGGGGGGGGG
  //       >person2_read2
  //       TTGACG


  //first set up the hash/graph
  int kmer_size = 3;
  int number_of_bits = 4;
  int bucket_size    = 4;
  long long bad_reads = 0;
  int max_retries=10;

  dBGraph * hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);

  
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }

  long long seq_loaded=0;

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/test_pop_load_and_print/two_individuals_simple.txt", &bad_reads, hash_table);
  //printf("Number of bases loaded is %d",seq_loaded);
  CU_ASSERT(seq_loaded == 44);
  CU_ASSERT(bad_reads==0);

  int stats[6]; //array of ints. stats[0] - number of kmers shared by noone; stats[1], number shared by one person, etc
  stats[0]=0; stats[1]=0; stats[2]=0; stats[3]=0; stats[4]=0; stats[5]=0; 

  int* array[3];
  array[0]=&stats[0];
  array[1]=&stats[1];
  array[2]=&stats[2];

  int num_people=2;

  db_graph_traverse_to_gather_statistics_about_people(&find_out_how_many_individuals_share_this_node_and_add_to_statistics , hash_table, array, num_people);

  CU_ASSERT(stats[0]==0);
  CU_ASSERT(stats[1]==6);
  CU_ASSERT(stats[2]==1);

  hash_table_free(&hash_table);
 
}


void test_get_min_and_max_covg_of_nodes_in_supernode()
{

  //first set up the hash/graph
  int kmer_size = 3;
  int number_of_bits = 4;
  int bucket_size    = 4;
  long long bad_reads = 0;
  int max_retries=10;

  dBGraph * hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);

  
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }

  long long seq_loaded=0;



  //start with an example with just oner person - note this is an example with the same kmer occuring twice in one read :) - CCG
  // so need to make sure that coverga eof that kmer is not double-incremented for the two times in that read

  // >r1
  //AACCGG
  //>r2
  //AAC
  //>r3
  //AAC
  //>r4
  //AAC
  //>r5
  //ACC
  //>r6
  //CCG
  //>r7
  //CCG

  //supernode will be
  // AAC --> ACC --> CCG ___|
  // TTG <-- TGG <-- GGC <--|
  // hairpin :-)

  //double coverness of hairpin must not confuse read coverage


  seq_loaded = load_population_as_fasta("../data/test/pop_graph/coverage/one_person", &bad_reads, hash_table);
  //printf("Number of bases loaded is %d",seq_loaded);
  CU_ASSERT(seq_loaded == 24);
  CU_ASSERT(bad_reads==0);
  

  dBNode* query_node = hash_table_find(element_get_key(seq_to_binary_kmer("AAC",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_get_coverage(query_node, individual_edge_array, 0)==4);
  //printf("Expect covg aac is 4, but is %d", db_node_get_coverage(query_node, individual_edge_array, 0));
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("ACC",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_get_coverage(query_node, individual_edge_array, 0)==2);
  //printf("Expect covg acc is 2, but is %d", db_node_get_coverage(query_node, individual_edge_array, 0));
  query_node = hash_table_find(element_get_key(seq_to_binary_kmer("CCG",hash_table->kmer_size), hash_table->kmer_size), hash_table);
  CU_ASSERT(!(query_node==NULL));
  CU_ASSERT(db_node_get_coverage(query_node, individual_edge_array, 0)==4); //NOTE this is only 4 because we cound kmer coverage. CCG occurs twice in read r1, once on each strand
  //printf("Expect covg ccg is 4, but is %d", db_node_get_coverage(query_node, individual_edge_array, 0));


  hash_table_free(&hash_table);

}


void test_db_graph_load_array_with_next_batch_of_nodes_corresponding_to_consecutive_bases_in_a_chrom_fasta()
{

  int length_of_arrays=14;
  int number_of_nodes_to_load=7;


  //first set up the hash/graph
  int kmer_size = 7;
  int number_of_bits = 8;
  int bucket_size    = 10;
  long long bad_reads = 0;
  int max_retries=10;

  dBGraph * db_graph = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  if (db_graph==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }
  long long seq_loaded=0;

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/one_person_for_testing_array_loading", &bad_reads, db_graph);

  //>one read
  //AATAGACGCCCACACCTGATAGACCCCACACTCTAA

  
  CU_ASSERT(seq_loaded==36);
  
  FILE* chrom_fptr = fopen("../data/test/pop_graph/one_person.fasta", "r");
  if (chrom_fptr==NULL)
    {
      printf("Cannot open ./data/test/pop_graph/one_person.fasta\n");
      exit(1);
    }

  //*************************************
  // malloc and initialising
  //*************************************
  dBNode**     chrom_path_array        = (dBNode**) malloc(sizeof(dBNode*)*length_of_arrays); //everything these dBNode*'s point to will be in the hash table - ie owned by the hash table
  Orientation* chrom_orientation_array = (Orientation*) malloc(sizeof(Orientation)*length_of_arrays); 
  Nucleotide*  chrom_labels         = (Nucleotide*) malloc(sizeof(Nucleotide)*length_of_arrays);
  char*        chrom_string            = (char*) malloc(sizeof(char)*length_of_arrays+1); //+1 for \0

  int n;
  for (n=0; n<length_of_arrays; n++)
    {
      chrom_path_array[n]=NULL;
      chrom_orientation_array[n]=forward;
      chrom_labels[n]=Undefined;
      chrom_string[n]='N';
    }
  chrom_string[0]='\0';


  Sequence * seq = malloc(sizeof(Sequence));
  if (seq == NULL){
    fputs("Out of memory trying to allocate Sequence\n",stderr);
    exit(1);
  }
  alloc_sequence(seq,number_of_nodes_to_load+db_graph->kmer_size+1,LINE_MAX);
  seq->seq[0]='\0';


  KmerSlidingWindow* kmer_window = malloc(sizeof(KmerSlidingWindow));
  if (kmer_window==NULL)
    {
      printf("Failed to malloc kmer sliding window . Exit.\n");
      exit(1);
    }
  kmer_window->kmer = (BinaryKmer*) malloc(sizeof(BinaryKmer)*1000);    //*(number_of_nodes_to_load + db_graph->kmer_size));
  if (kmer_window->kmer==NULL)
    {
      printf("Failed to malloc kmer_window->kmer ");
      exit(1);
    }
  
  kmer_window->nkmers=0;

  // ***********************************************************
  //********** end of malloc and initialise ********************



  int ret = db_graph_load_array_with_next_batch_of_nodes_corresponding_to_consecutive_bases_in_a_chrom_fasta(chrom_fptr, number_of_nodes_to_load, 0, 
													     length_of_arrays,
													     chrom_path_array, chrom_orientation_array, chrom_labels, chrom_string,
													     seq, kmer_window, 
													     true, false,
													     db_graph);
  CU_ASSERT(ret==number_of_nodes_to_load);

  char tmp_seqzam[db_graph->kmer_size+1];
  tmp_seqzam[db_graph->kmer_size]='\0';


  int i;
  //length of arrays is 14, number of nodes to load is 7
  for(i=0; i<length_of_arrays-number_of_nodes_to_load; i++)
    {
      CU_ASSERT(chrom_path_array[i]==NULL);
    }
  CU_ASSERT_STRING_EQUAL( "AATAGAC", binary_kmer_to_seq(chrom_path_array[7]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ATAGACG", binary_kmer_to_seq(chrom_path_array[8]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GCGTCTA", binary_kmer_to_seq(chrom_path_array[9]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGACGCC", binary_kmer_to_seq(chrom_path_array[10]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GACGCCC", binary_kmer_to_seq(chrom_path_array[11]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACGCCCA", binary_kmer_to_seq(chrom_path_array[12]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CGCCCAC", binary_kmer_to_seq(chrom_path_array[13]->kmer, db_graph->kmer_size, tmp_seqzam) );


  //one more batch, then array is full,
  ret = db_graph_load_array_with_next_batch_of_nodes_corresponding_to_consecutive_bases_in_a_chrom_fasta(chrom_fptr, number_of_nodes_to_load, number_of_nodes_to_load, 
													 length_of_arrays,
													 chrom_path_array, chrom_orientation_array, chrom_labels, chrom_string,
													 seq, kmer_window, 
													 false, false,
													 db_graph);
  CU_ASSERT(ret==number_of_nodes_to_load);



  CU_ASSERT_STRING_EQUAL( "AATAGAC", binary_kmer_to_seq(chrom_path_array[0]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ATAGACG", binary_kmer_to_seq(chrom_path_array[1]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GCGTCTA", binary_kmer_to_seq(chrom_path_array[2]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGACGCC", binary_kmer_to_seq(chrom_path_array[3]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GACGCCC", binary_kmer_to_seq(chrom_path_array[4]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACGCCCA", binary_kmer_to_seq(chrom_path_array[5]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CGCCCAC", binary_kmer_to_seq(chrom_path_array[6]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GCCCACA", binary_kmer_to_seq(chrom_path_array[7]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCCACAC", binary_kmer_to_seq(chrom_path_array[8]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCACACC", binary_kmer_to_seq(chrom_path_array[9]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGGTGTG", binary_kmer_to_seq(chrom_path_array[10]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACACCTG", binary_kmer_to_seq(chrom_path_array[11]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CACCTGA", binary_kmer_to_seq(chrom_path_array[12]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACCTGAT", binary_kmer_to_seq(chrom_path_array[13]->kmer, db_graph->kmer_size, tmp_seqzam) );
  

  //from now on, it is always true that LAST time was not a new fasta entry, so penultimate argument is TRUE
  ret = db_graph_load_array_with_next_batch_of_nodes_corresponding_to_consecutive_bases_in_a_chrom_fasta(chrom_fptr, number_of_nodes_to_load, number_of_nodes_to_load, 
													 length_of_arrays,
													 chrom_path_array, chrom_orientation_array, chrom_labels, chrom_string,
													 seq, kmer_window, 
													 false, true,
                                                                                                         db_graph);

  CU_ASSERT(ret==number_of_nodes_to_load);


  CU_ASSERT_STRING_EQUAL( "GCCCACA", binary_kmer_to_seq(chrom_path_array[0]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCCACAC", binary_kmer_to_seq(chrom_path_array[1]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCACACC", binary_kmer_to_seq(chrom_path_array[2]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGGTGTG", binary_kmer_to_seq(chrom_path_array[3]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACACCTG", binary_kmer_to_seq(chrom_path_array[4]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CACCTGA", binary_kmer_to_seq(chrom_path_array[5]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACCTGAT", binary_kmer_to_seq(chrom_path_array[6]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCTGATA", binary_kmer_to_seq(chrom_path_array[7]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CTATCAG", binary_kmer_to_seq(chrom_path_array[8]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "TCTATCA", binary_kmer_to_seq(chrom_path_array[9]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GATAGAC", binary_kmer_to_seq(chrom_path_array[10]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ATAGACC", binary_kmer_to_seq(chrom_path_array[11]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GGGTCTA", binary_kmer_to_seq(chrom_path_array[12]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGACCCC", binary_kmer_to_seq(chrom_path_array[13]->kmer, db_graph->kmer_size, tmp_seqzam) );
  

  //and again

  ret = db_graph_load_array_with_next_batch_of_nodes_corresponding_to_consecutive_bases_in_a_chrom_fasta(chrom_fptr, number_of_nodes_to_load, number_of_nodes_to_load, 
													 length_of_arrays,
													 chrom_path_array, chrom_orientation_array, chrom_labels, chrom_string,
													 seq, kmer_window, 
													 false, true,
                                                                                                         db_graph);

  CU_ASSERT(ret==number_of_nodes_to_load);


  CU_ASSERT_STRING_EQUAL( "CCTGATA", binary_kmer_to_seq(chrom_path_array[0]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CTATCAG", binary_kmer_to_seq(chrom_path_array[1]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "TCTATCA", binary_kmer_to_seq(chrom_path_array[2]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GATAGAC", binary_kmer_to_seq(chrom_path_array[3]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ATAGACC", binary_kmer_to_seq(chrom_path_array[4]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "GGGTCTA", binary_kmer_to_seq(chrom_path_array[5]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGACCCC", binary_kmer_to_seq(chrom_path_array[6]->kmer, db_graph->kmer_size, tmp_seqzam) );

  CU_ASSERT_STRING_EQUAL( "GACCCCA", binary_kmer_to_seq(chrom_path_array[7]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACCCCAC", binary_kmer_to_seq(chrom_path_array[8]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCCCACA", binary_kmer_to_seq(chrom_path_array[9]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCCACAC", binary_kmer_to_seq(chrom_path_array[10]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGTGTGG", binary_kmer_to_seq(chrom_path_array[11]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CACACTC", binary_kmer_to_seq(chrom_path_array[12]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACACTCT", binary_kmer_to_seq(chrom_path_array[13]->kmer, db_graph->kmer_size, tmp_seqzam) );


  //now - does it cope with hitting the end of the entry before getting the required number of nodes

  ret = db_graph_load_array_with_next_batch_of_nodes_corresponding_to_consecutive_bases_in_a_chrom_fasta(chrom_fptr, number_of_nodes_to_load, number_of_nodes_to_load, 
													 length_of_arrays,
													 chrom_path_array, chrom_orientation_array, chrom_labels, chrom_string,
													 seq, kmer_window, 
													 false, true,
                                                                                                         db_graph);

  CU_ASSERT(ret==2);

  CU_ASSERT_STRING_EQUAL( "GACCCCA", binary_kmer_to_seq(chrom_path_array[0]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACCCCAC", binary_kmer_to_seq(chrom_path_array[1]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCCCACA", binary_kmer_to_seq(chrom_path_array[2]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CCCACAC", binary_kmer_to_seq(chrom_path_array[3]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "AGTGTGG", binary_kmer_to_seq(chrom_path_array[4]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "CACACTC", binary_kmer_to_seq(chrom_path_array[5]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACACTCT", binary_kmer_to_seq(chrom_path_array[6]->kmer, db_graph->kmer_size, tmp_seqzam) );

  CU_ASSERT_STRING_EQUAL( "CACTCTA", binary_kmer_to_seq(chrom_path_array[7]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT_STRING_EQUAL( "ACTCTAA", binary_kmer_to_seq(chrom_path_array[8]->kmer, db_graph->kmer_size, tmp_seqzam) );
  CU_ASSERT(chrom_path_array[9]==NULL);
  CU_ASSERT(chrom_path_array[10]==NULL);
  CU_ASSERT(chrom_path_array[11]==NULL);
  CU_ASSERT(chrom_path_array[12]==NULL);
  CU_ASSERT(chrom_path_array[13]==NULL);



  //cleanup

  free(chrom_path_array);
  free(chrom_orientation_array);
  free(chrom_string);
  free(chrom_labels);
  free_sequence(&seq);
  hash_table_free(&db_graph);
  fclose(chrom_fptr);

}

void test_db_graph_make_reference_path_based_sv_calls()
{

  // ******************************************************************************************************************************
  // 1. NULL test. Load short fake reference, and load a person whose sequence is identical. This should find nothing.
  // ******************************************************************************************************************************

  printf("Start subtest 1\n");

  // toy example  - kmer_size=7
  //first set up the hash/graph
  int kmer_size = 7;
  int number_of_bits = 8;
  int bucket_size    = 10;
  long long bad_reads = 0;
  int max_retries=10;

  dBGraph * hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }
  long long seq_loaded=0;

  //loads two people into individual_edge_array, with identical fasta containing one read of length 31 bases.
  //notionally, one of these people is the reference. We will never use this person/references set of edges except 
  // when we go through their fasta again, and do hash)table_find for each kmer we find in the fasta, in order, and generate an array of nodes etc.

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/two_people_each_with_the_same_read", &bad_reads, hash_table);

  //>one read
  //AATAGACGCCCACACCTGATAGACCCCACAC 


  // The de Bruijn graph of this in 7mers is as follows:
  // a line of 8 nodes, with a loop of length 16 off the 9th node. ie all nodes have 1-in and 1-out, except the 9th node, CCCACAC, which has 2 ins and 2 outs.
  // so it's a supernode of length 8, a loop-supernode of length 16,

  // our algorithm should look at the supernode starting with AATAGAC, then the supernode starting at CCCACAC, then stop


  
  CU_ASSERT(seq_loaded==62);
  
  FILE* chrom_fptr = fopen("../data/test/pop_graph/second_person_with_same_read.fasta", "r");
  if (chrom_fptr==NULL)
    {
      printf("Cannot open ./data/test/pop_graph/second_person_with_same_read.fasta\n");
      exit(1);
    }

  int min_fiveprime_flank_anchor = 2;
  int min_threeprime_flank_anchor= 3;// I want to try to attach anchors, and the supernodes are quite short, so for this test only ask for (ridiculously) small anchors
  int max_anchor_span = 7;
  int length_of_arrays=14;
  int min_covg =1;
  int max_covg = 10;
  int ret = db_graph_make_reference_path_based_sv_calls(chrom_fptr, individual_edge_array, 0, 
							min_fiveprime_flank_anchor, min_threeprime_flank_anchor, max_anchor_span, min_covg, max_covg, 
							length_of_arrays, hash_table, NULL );

  CU_ASSERT(ret==0);

  hash_table_free(&hash_table);
  fclose(chrom_fptr);

  /*



  // ******************************************************************************************************************************
  // 2. Harder NULL test. Reference=an ALU and load a person whose sequence is also that ALU. This should find nothing.
  // ******************************************************************************************************************************

  printf("Start subtest 2\n");


  kmer_size = 31;
  number_of_bits = 8;
  bucket_size    = 10;
  bad_reads = 0;
  max_retries=10;

  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }
  seq_loaded=0;

  //just load one person who's sequence is an Alu. Then take reference which is that same sequence and try to find variants
  seq_loaded = load_population_as_fasta("../data/test/pop_graph/test_pop_load_and_print/two_people_sharing_alu/just_one_of_the_two_people.txt", &bad_reads, hash_table);

  //   >7SLRNA#SINE/Alu  plus GTTCAGAG at start and GTCAGCGTAG at end
  //   GTTCAGAGGCCGGGCGCGGTGGCGCGTGCCTGTAGTCCCAGCTACTCGGGAGGCTGAG
  //   GTGGGAGGATCGCTTGAGTCCAGGAGTTCTGGGCTGTAGTGCGCTATGCC
  //   GATCGGGTGTCCGCACTAAGTTCGGCATCAATATGGTGACCTCCCGGGAG
  //   CGGGGGACCACCAGGTTGCCTAAGGAGGGGTGAACCGGCCCAGGTCGGAA
  //   ACGGAGCAGGTCAAAACTCCCGTGCTGATCAGTAGTGGGATCGCGCCTGT
  //   GAATAGCCACTGCACTCCAGCCTGAGCAACATAGCGAGACCCCGTCTCTT
  //   AAAAAAAAAAAAAAAAAAAAGTCAGCCGTAG
    
   
  
  CU_ASSERT(seq_loaded==339);
  
  chrom_fptr = fopen("../data/test/pop_graph/test_pop_load_and_print/two_people_sharing_alu/person1.fasta", "r");
  if (chrom_fptr==NULL)
    {
      printf("Cannot open ../data/test/pop_graph/test_pop_load_and_print/two_people_sharing_alu/person1.fasta");
      exit(1);
    }

  min_fiveprime_flank_anchor = 10;
  min_threeprime_flank_anchor= 10;
  max_anchor_span = 50;
  length_of_arrays=100;
  min_covg =1;
  max_covg = 10;
  ret = db_graph_make_reference_path_based_sv_calls(chrom_fptr, individual_edge_array, 0, 
						    min_fiveprime_flank_anchor, min_threeprime_flank_anchor, max_anchor_span, min_covg, max_covg, 
						    length_of_arrays, hash_table, NULL );
  
  CU_ASSERT(ret==0);

  hash_table_free(&hash_table);
  fclose(chrom_fptr);


  // ******************************************************************************************************************************
  // 3. Reference = Alu-NNNNN- same Alu, and person is identical to reference. This should find nothing
  // ******************************************************************************************************************************

  printf("Start subtest 3\n");


  kmer_size = 31;
  number_of_bits = 8;
  bucket_size    = 10;
  bad_reads = 0;
  max_retries=10;

  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }
  seq_loaded=0;

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/variations/one_person_is_alu_Ns_then_same_alu", &bad_reads, hash_table);

  
  //   >7SLRNA#SINE/Alu  
  // GTTCAGAGGCCGGGCGCGGTGGCGCGTGCCTGTAGTCCCAGCTACTCGGGAGGCTGAG
  // GTGGGAGGATCGCTTGAGTCCAGGAGTTCTGGGCTGTAGTGCGCTATGCC
  // GATCGGGTGTCCGCACTAAGTTCGGCATCAATATGGTGACCTCCCGGGAG
  // CGGGGGACCACCAGGTTGCCTAAGGAGGGGTGAACCGGCCCAGGTCGGAA
  // ACGGAGCAGGTCAAAACTCCCGTGCTGATCAGTAGTGGGATCGCGCCTGT
  // GAATAGCCACTGCACTCCAGCCTGAGCAACATAGCGAGACCCCGTCTCTT
  // AAAAAAAAAAAAAAAAAAAAGTCAGCCGTAGNNNNNNNNNNNNNNNNNNN
  // GTTCAGAGGCCGGGCGCGGTGGCGCGTGCCTGTAGTCCCAGCTACTCGGGAGGCTGAG
  // GTGGGAGGATCGCTTGAGTCCAGGAGTTCTGGGCTGTAGTGCGCTATGCC
  // GATCGGGTGTCCGCACTAAGTTCGGCATCAATATGGTGACCTCCCGGGAG
  // CGGGGGACCACCAGGTTGCCTAAGGAGGGGTGAACCGGCCCAGGTCGGAA
  // ACGGAGCAGGTCAAAACTCCCGTGCTGATCAGTAGTGGGATCGCGCCTGT
  // GAATAGCCACTGCACTCCAGCCTGAGCAACATAGCGAGACCCCGTCTCTT
  // AAAAAAAAAAAAAAAAAAAAGTCAGCCGTAG
    
  
  
  CU_ASSERT(seq_loaded==697);
  
  chrom_fptr = fopen("../data/test/pop_graph/variations/one_person_aluNsalu.fasta", "r");
  if (chrom_fptr==NULL)
    {
      printf("Cannot open ../data/test/pop_graph/variations/one_person_aluNsalu.fasta");
      exit(1);
    }

  min_fiveprime_flank_anchor = 10;
  min_threeprime_flank_anchor= 10;
  max_anchor_span = 50;
  length_of_arrays=100;
  min_covg =1;
  max_covg = 10;
  ret = db_graph_make_reference_path_based_sv_calls(chrom_fptr, individual_edge_array, 0, 
						    min_fiveprime_flank_anchor, min_threeprime_flank_anchor, max_anchor_span, min_covg, max_covg, 
						    length_of_arrays, hash_table, NULL );
  
  CU_ASSERT(ret==0);

  hash_table_free(&hash_table);
  fclose(chrom_fptr);


  // ******************************************************************************************************************************
  // 4. Reference = 10kb of chromosome 1. Individual is that same sequence. This should find nothing.
  // ******************************************************************************************************************************

  printf("Start subtest 4\n");

  kmer_size = 31;
  number_of_bits = 13;
  bucket_size    = 10;
  bad_reads = 0;
  max_retries=10;

  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }
  seq_loaded=0;

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/variations/one_person_is_10kb_of_chrom1", &bad_reads, hash_table);

  
  CU_ASSERT(seq_loaded==11940);
  
  chrom_fptr = fopen("../data/test/pop_graph/variations/person_1kb_chrom1.fasta", "r");
  if (chrom_fptr==NULL)
    {
      printf("Cannot open ../data/test/pop_graph/variations/person_1kb_chrom1.fasta");
      exit(1);
    }

  min_fiveprime_flank_anchor = 20;
  min_threeprime_flank_anchor= 10;
  max_anchor_span = 1000;
  length_of_arrays=2000;
  min_covg =1;
  max_covg = 10;
  ret = db_graph_make_reference_path_based_sv_calls(chrom_fptr, individual_edge_array, 0, 
						    min_fiveprime_flank_anchor, min_threeprime_flank_anchor, max_anchor_span, min_covg, max_covg, 
						    length_of_arrays, hash_table, NULL );
  
  CU_ASSERT(ret==0);

  hash_table_free(&hash_table);
  fclose(chrom_fptr);






  // ***************************************************************************************************************************************************
  // 5. Reference = Alu-NNNNN- same Alu, and person is identical to reference except for a single base difference. This should find the single variant!
  // ***************************************************************************************************************************************************

  printf("Start subtest 5\n");

  kmer_size = 31;
  number_of_bits = 8;
  bucket_size    = 10;
  bad_reads = 0;
  max_retries=10;

  hash_table = hash_table_new(number_of_bits,bucket_size,max_retries,kmer_size);
  if (hash_table==NULL)
    {
      printf("unable to alloc the hash table. dead before we even started. OOM");
      exit(1);
    }
  seq_loaded=0;

  seq_loaded = load_population_as_fasta("../data/test/pop_graph/variations/one_person_is_alu_Ns_then_same_alu", &bad_reads, hash_table);

  CU_ASSERT(seq_loaded==697);
  
  chrom_fptr = fopen("../data/test/pop_graph/variations/one_person_aluNsalu_PLUS_SINGLE_BASE_CHANGE.fasta", "r");
  if (chrom_fptr==NULL)
    {
      printf("Cannot open ../data/test/pop_graph/variations/one_person_aluNsalu_PLUS_SINGLE_BASE_CHANGE.fasta");
      exit(1);
    }

  min_fiveprime_flank_anchor = 10;
  min_threeprime_flank_anchor= 10;
  max_anchor_span =100;
  length_of_arrays=200;
  min_covg =1;
  max_covg = 10;
  ret = db_graph_make_reference_path_based_sv_calls(chrom_fptr, individual_edge_array, 0, 
						    min_fiveprime_flank_anchor, min_threeprime_flank_anchor, max_anchor_span, min_covg, max_covg, 
						    length_of_arrays, hash_table, NULL );
  
  CU_ASSERT(ret==1);
  printf("Ret is %d\n", ret);
  hash_table_free(&hash_table);
  fclose(chrom_fptr);


*/




  // ?. Load 1kb of chrom 1 as reference, and a person whose graph contains a single, long loop, and an AAAAAA loop. Do we get trapped in an infinite loop?

  // ? As above, but with a person whose graph is identical to chrom 1 but for a single SNP, PLUS these loops. Do we find the SNP?



}
