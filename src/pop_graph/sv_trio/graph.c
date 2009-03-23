#include <element.h>
#include <stdio.h>
#include <stdlib.h>
#include <file_reader.h>
#include <dB_graph.h>
#include <dB_graph_population.h>

int main(int argc, char **argv){

  printf("START\n");

  FILE *fp_fnames;
  //char filename[100];
  int hash_key_bits;
  dBGraph * db_graph = NULL; 
  short kmer_size;


  //command line arguments 
  //filename = argv[1];    //open file that lists one file per individual in the trio (population), and each of those gives list of files.
  kmer_size        = atoi(argv[2]);  //global variable defined in element.h
  hash_key_bits    = atoi(argv[3]); //number of buckets: 2^hash_key_bits
  DEBUG            = atoi(argv[4]);

  fprintf(stderr,"Kmer size: %d hash_table_size (%d bits): %d\n",kmer_size,hash_key_bits,1 << hash_key_bits);

  //Create the de Bruijn graph/hash table
  db_graph = hash_table_new(1 << hash_key_bits,kmer_size);
  fprintf(stderr,"table created: %d\n",1 << hash_key_bits);

  long count_files = 0;
  long long bad_reads =0;
  long long total_kmers =0;
  long long total_length = 0; //total sequence length

  //Go through all the files, loading data into the graph
  total_length += load_population_as_fastq(argv[1], &total_kmers, &bad_reads, 30, db_graph);
  fprintf(stderr,"\n%ld file name:%s total seq:%lld bad reads: %lld total kmers:%lld\n\n",count_files,argv[1],total_length, bad_reads, total_kmers);
  printf("\nLoaded all the files\n %ld file name:%s total seq:%lld bad reads: %lld total kmers:%lld\n\n",count_files,argv[1],total_length, bad_reads, total_kmers);

  //now load the chromosomes
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.1.fa.short_reads" , db_graph, 1);
  printf("Loaded chromosome 1\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.2.fa.short_reads" , db_graph, 2);
  printf("Loaded chromosome 2\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.3.fa.short_reads" , db_graph, 3);
  printf("Loaded chromosome 3\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.4.fa.short_reads" , db_graph, 4);
  printf("Loaded chromosome 4\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.5.fa.short_reads" , db_graph, 5);
  printf("Loaded chromosome 5\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.6.fa.short_reads" , db_graph, 6);
  printf("Loaded chromosome 6\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.7.fa.short_reads" , db_graph, 7);
  printf("Loaded chromosome 7\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.8.fa.short_reads" , db_graph, 8);
  printf("Loaded chromosome 8\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.9.fa.short_reads" , db_graph, 9);
  printf("Loaded chromosome 9\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.10.fa.short_reads" , db_graph, 10);
  printf("Loaded chromosome 10\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.11.fa.short_reads" , db_graph, 11);
  printf("Loaded chromosome 11\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.12.fa.short_reads" , db_graph, 12);
  printf("Loaded chromosome 12\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.13.fa.short_reads" , db_graph, 13);
  printf("Loaded chromosome 13\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.14.fa.short_reads" , db_graph, 14);
  printf("Loaded chromosome 14\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.15.fa.short_reads" , db_graph, 15);
  printf("Loaded chromosome 15\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.16.fa.short_reads" , db_graph, 16);
  printf("Loaded chromosome 16\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.17.fa.short_reads" , db_graph, 17);
  printf("Loaded chromosome 17\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.18.fa.short_reads" , db_graph, 18);
  printf("Loaded chromosome 18\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.19.fa.short_reads" , db_graph, 19);
  printf("Loaded chromosome 19\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.20.fa.short_reads" , db_graph, 20);
  printf("Loaded chromosome 20\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.21.fa.short_reads" , db_graph, 21);
  printf("Loaded chromosome 21\n");
  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.22.fa.short_reads" , db_graph, 22);
  printf("Loaded chromosome 22\n");
  //load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.23.fa.short_reads" , db_graph, 23);
  //printf("Loaded chromosome 23\n");
  //  load_chromosome_overlap_data("/nfs/1000g-work/G1K/work/zi/projects/marzam/humref/split/clean/Homo_sapiens.NCBI36.52.dna.chromosome.24.fa.short_reads" , db_graph, 24);
  //printf("Loaded chromosome 24\n");

  printf("All Chromosomes loaded. Start printing out potential inversions/indels\n");

  //remember last 4 arguments are only used for unit tests, so here they are NULL, and 0.
  long supernode_count0=0;
  db_graph_traverse_specific_person_or_pop_for_supernode_and_chromosome_overlap_printing(&db_graph_choose_output_filename_and_print_potential_inversion_for_specific_person_or_pop,db_graph,&supernode_count0, 
								  individual_edge_array,0, false,NULL,NULL,0,0);

  printf("printed out all the possible iversion sites we can find. start on translocs\n");
  db_graph_set_all_visited_nodes_to_status_none(db_graph);

  db_graph_traverse_specific_person_or_pop_for_supernode_and_chromosome_overlap_printing(&db_graph_choose_output_filename_and_print_potential_transloc_for_specific_person_or_pop,db_graph,&supernode_count0, 
								  individual_edge_array,0, false,NULL,NULL,0,0);
  printf("printed out all the possible transloc sites we can find\n");

  db_graph_set_all_visited_nodes_to_status_none(db_graph);

  printf("Finished\n");
  return 1;
}
