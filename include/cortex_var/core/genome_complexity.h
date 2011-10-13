/*
 * Copyright 2009-2011 Zamin Iqbal and Mario Caccamo  
 * 
 * CORTEX project contacts:  
 * 		M. Caccamo (mario.caccamo@bbsrc.ac.uk) and 
 * 		Z. Iqbal (zam@well.ox.ac.uk)
 *
 * **********************************************************************
 *
 * This file is part of CORTEX.
 *
 * CORTEX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CORTEX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CORTEX.  If not, see <http://www.gnu.org/licenses/>.
 *
 * **********************************************************************
 */

#ifndef GENOME_COMPLEXITY_H
#define GENOME_COMPLEXITY_H

#include <dB_graph.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <file_reader.h>
#include <seq.h>
#include <file_format.h>


#define MAX_READLEN_FOR_GEN_COMPLEXITY 1000
#define MAX_NUM_READS_USED_FOR_ESTIMATE 10000

boolean allele_is_clean(dBNode** array_nodes,Orientation* array_or, 
			int len, int colour_cleaned_genome, int kmer);


void get_clean_and_unclean_counts(dBGraph* db_graph, char* fasta, boolean allow_reads_shorter_than_2k_plus_one, 
				  int colour_cleaned_genome, 
				  int* total_errors_tested, int* total_errors_forming_clean_bubbles,
				  int (*file_reader)(FILE * fp, Sequence * seq, int max_read_length, boolean new_entry, boolean * full_entry), 
				  dBNode** array_nodes, Orientation* array_or, //assume these are length max_read_length+k+1 - plenty of space
				  Sequence* seq, KmerSlidingWindow* kmer_window, int max_read_length);


double estimate_genome_complexity(dBGraph* db_graph, char* filename_list_fastaq,
				  boolean allow_reads_shorter_than_2k_plus_one, 
				  int colour_cleaned_genome,
				  int max_read_length, FileFormat format,
				  int fastq_ascii_offset
				  );

#endif
