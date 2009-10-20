#include <CUnit.h>
#include <Basic.h>
#include <element.h>
#include <test_graph_element.h>

void test_graph_element_assign()
{
  Element e1, e2;

  BinaryKmer b1;
  binary_kmer_initialise_to_zero(&b1);
  b1[NUMBER_OF_BITFIELDS_IN_BINARY_KMER-1] = (bitfield_of_64bits) 1;

  BinaryKmer b2;
  binary_kmer_initialise_to_zero(&b2);
  b2[NUMBER_OF_BITFIELDS_IN_BINARY_KMER-1] = (bitfield_of_64bits) 3;

  
  element_initialise(&e1, &b1, 31);
  element_initialise(&e2, &b2, 31);

  db_node_set_edges(&e1, 1);
  db_node_set_edges(&e2, 2);
  
  db_node_set_status(&e1, visited);
  db_node_set_status(&e2, pruned);

  element_update_coverage(&e1, 101);
  element_update_coverage(&e2, 202);
  
  element_assign(&e1, &e2);

  CU_ASSERT(e1.coverage==202);
  CU_ASSERT(e1.edges==2 );
  CU_ASSERT(binary_kmer_comparison_operator(e1.kmer,b2) );
  CU_ASSERT(e1.status==pruned );
}