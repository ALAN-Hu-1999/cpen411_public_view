/* sim-safe.c - sample functional simulator implementation */

/* SimpleScalar(TM) Tool Suite
 * Copyright (C) 1994-2003 by Todd M. Austin, Ph.D. and SimpleScalar, LLC.
 * All Rights Reserved. 
 * 
 * THIS IS A LEGAL DOCUMENT, BY USING SIMPLESCALAR,
 * YOU ARE AGREEING TO THESE TERMS AND CONDITIONS.
 * 
 * No portion of this work may be used by any commercial entity, or for any
 * commercial purpose, without the prior, written permission of SimpleScalar,
 * LLC (info@simplescalar.com). Nonprofit and noncommercial use is permitted
 * as described below.
 * 
 * 1. SimpleScalar is provided AS IS, with no warranty of any kind, express
 * or implied. The user of the program accepts full responsibility for the
 * application of the program and the use of any results.
 * 
 * 2. Nonprofit and noncommercial use is encouraged. SimpleScalar may be
 * downloaded, compiled, executed, copied, and modified solely for nonprofit,
 * educational, noncommercial research, and noncommercial scholarship
 * purposes provided that this notice in its entirety accompanies all copies.
 * Copies of the modified software can be delivered to persons who use it
 * solely for nonprofit, educational, noncommercial research, and
 * noncommercial scholarship purposes provided that this notice in its
 * entirety accompanies all copies.
 * 
 * 3. ALL COMMERCIAL USE, AND ALL USE BY FOR PROFIT ENTITIES, IS EXPRESSLY
 * PROHIBITED WITHOUT A LICENSE FROM SIMPLESCALAR, LLC (info@simplescalar.com).
 * 
 * 4. No nonprofit user may place any restrictions on the use of this software,
 * including as modified by the user, by any other authorized user.
 * 
 * 5. Noncommercial and nonprofit users may distribute copies of SimpleScalar
 * in compiled or executable form as set forth in Section 2, provided that
 * either: (A) it is accompanied by the corresponding machine-readable source
 * code, or (B) it is accompanied by a written offer, with no time limit, to
 * give anyone a machine-readable copy of the corresponding source code in
 * return for reimbursement of the cost of distribution. This written offer
 * must permit verbatim duplication by anyone, or (C) it is distributed by
 * someone who received only the executable form, and is accompanied by a
 * copy of the written offer of source code.
 * 
 * 6. SimpleScalar was developed by Todd M. Austin, Ph.D. The tool suite is
 * currently maintained by SimpleScalar LLC (info@simplescalar.com). US Mail:
 * 2395 Timbercrest Court, Ann Arbor, MI 48105.
 * 
 * Copyright (C) 1994-2003 by Todd M. Austin, Ph.D. and SimpleScalar, LLC.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "host.h"
#include "misc.h"
#include "machine.h"
#include "regs.h"
#include "memory.h"
#include "loader.h"
#include "syscall.h"
#include "options.h"
#include "stats.h"
#include "sim.h"

/*
 * This file implements a functional simulator.  This functional simulator is
 * the simplest, most user-friendly simulator in the simplescalar tool set.
 * Unlike sim-fast, this functional simulator checks for all instruction
 * errors, and the implementation is crafted for clarity rather than speed.
 */

/***** counters *****/
<<<<<<< HEAD
static counter_t g_total_dm_store_hit; 
static counter_t g_total_dm_load_hit; 
static counter_t g_total_dm_store_miss;
static counter_t g_total_dm_load_miss;
static counter_t g_total_dm_victim_store_hit; 
static counter_t g_total_dm_victim_load_hit; 
static counter_t g_total_dm_victim_store_miss;
static counter_t g_total_dm_victim_load_miss;

static counter_t g_total_sa2w_store_hit;
static counter_t g_total_sa2w_load_hit;
static counter_t g_total_sa2w_store_miss;
static counter_t g_total_sa2w_load_miss;

static counter_t g_total_sa4w_store_hit;
static counter_t g_total_sa4w_load_hit;
static counter_t g_total_sa4w_store_miss;
static counter_t g_total_sa4w_load_miss;

static counter_t g_total_miss_store_sa8w_wb_cache;
static counter_t g_total_hit_store_sa8w_wb_cache;
static counter_t g_total_miss_load_sa8w_wb_cache;
static counter_t g_total_hit_load_sa48_wb_cache;
static counter_t g_total_write_back_event;

static counter_t g_total_sa8w_vic_store_miss;
static counter_t g_total_sa8w_vic_load_miss;  
static counter_t g_total_sa8w_vic_store_hit;
static counter_t g_total_sa8w_vic_load_hit; 

static counter_t g_total_store_inst = 0;
static counter_t g_total_load_inst = 0;





/* simulated registers */
static struct regs_t regs;

/***** simulate cache here *****/
<<<<<<< HEAD
// instrcution cache
// a direct-mapped cache with 32KB capacity and 64-byte blocks:
// since capacity is 32KB, block size is 64 byte
// there will be 512 row
struct dm_cache_t {
  int valid[512];
  int tag[512];
  int Data[512];
};

//2-way, 32KB, 64 byte
// we have 256 rows -> 8 bits index, + 6 bits offset
struct sa2w_cache_t
{
  int valid[2][256];
  int tag[2][256];
  int data[2][256];
  int LRU[256][2];
};


// 4-way set associative cache with 32KB capacity and 32-byte blocks and LRU replacement:
// since 4-way, 32-byte blocks, and capacity is 32KB
// so we need 32KB /4 ways / 256 rows
// implement pseudo-LRU for 4-way cache, so we need 3 bits
struct sa4w_cache_t {
  int LRU[256][4]; 
  int valid[4][256];  // 4 sets of valid bit
  int tag[4][256];    // 4 sets of tags
  int data[4][256];   // 4 sets of datas
};

// 8-way set-associative data cache with 32KB capacity and 64-byte blocks, 
// assuming a writeback policy with write allocate for store instructions and 
// LRU replacement.
// so we need 32KB / 64B / 8 = 64 rows
struct sa8w_wb_cache_t {
  int LRU[64][8];
  int dirty[8][64]; // 8 sets of dirty bit
  int valid[8][64]; // 8 sets of valid bit
  int tag[8][64];   // 8 sets of tags
  int data[8][64];  // 8 sets of datas 
};

// fully associative victim cache , with 2KB capacity and 64-byte block,
// LRU replacement 
// so we have 2KB / 64byte = 32 rows
// since block size is 64 byte, offset is 6 bits
// so tag size is 26 bits
struct victim_cache_t {
  int valid[32];
  int tag[32];
  int data[32];
};

/* simulated memory */
static struct mem_t *mem = NULL;

/* track number of refs */
static counter_t sim_num_refs = 0;

/* maximum number of inst's to execute */
static unsigned int max_insts;


/***** victim cache mapping*****/
// returns 1 if hit in victim cache, 0 if miss
int 
victimcache_map (struct victim_cache_t * victim_cache, int new_tag, int new_tag_replace){
  int i;
  int tag_matched = 0;
  int hit = 0;
  int hit_index = 0;
  for(i=0; i<32; i++){
    if(victim_cache->tag[i] == new_tag){
      tag_matched = 1;
      if(victim_cache->valid[i] == 1){
        hit = 1;
        hit_index = i;
        break;
      }
    }
  }

  // removed the matched tag (index at i)
  if(tag_matched == 1){
    int j;
    for(j=hit_index; j<32; j++){
      victim_cache->tag[j] = victim_cache->tag[j+1];
      victim_cache->valid[j] = victim_cache->valid[j+1];
    }
  }

  // place the replaced tag
  int replace_index =33;
  for(i = 0; i<32;i++){
    if(victim_cache->valid[i]==0){
      victim_cache->tag[i] = new_tag_replace;
      victim_cache->valid[i] = 1;
      replace_index = i;
      break;
    }
  }

  if(replace_index==33){
    // if the victim cache is full, remove the first tag in the array, put new tag at the end of the array
    int k;
    for(k = 0; k<31 ; k++){
      victim_cache->tag[k] = victim_cache->tag[k+1];
      victim_cache->valid[k] = victim_cache->valid[k+1];
    }
    victim_cache->tag[31] = new_tag_replace;
    victim_cache->valid[31] = 1;
  }

  return hit;
}


/***** dm cache mapping *****/
void 
dm_cache_map (struct dm_cache_t * dm_cache, struct victim_cache_t * victim_cache, int mem_addr, int is_write){
  // calculating index, tag, offset by the memory address
  // since we need 9 bits index for 512 rows, and 6 bits offset for 64B data
  // so tag size = 32 - 9 - 6 = 17
  int tag = mem_addr / 64 / 512;
  int index = mem_addr / 64 % 512;
  int offset = mem_addr % 64;

  // compare tag
  if(dm_cache->tag[index] == tag){
    if(dm_cache->valid[index] == 1){
      // tag matched, and the data is valid, it is a hit
      // increament hit counter
      if(is_write)  g_total_dm_store_hit++;
      else  g_total_dm_load_hit++;
    }
    else{
      // tag matched, but data not valid, it is a miss
      // increament miss counter, set valid to 1
      // ** need to update data
      dm_cache->valid[index] = 1;
      if(is_write)  g_total_dm_store_miss++;
      else  g_total_dm_load_miss++;    

    }
  }
  else{
    // the tag does not matched, it is a miss
    // increament miss counter, rewrite tag, set valid to 1
<<<<<<< HEAD
    if(is_write)  g_total_dm_store_miss++;
    else  g_total_dm_load_miss++;    
    
    //call victim cache
    int victim_hit = victimcache_map(victim_cache, (mem_addr/64), (dm_cache->tag[index]*512 + index));
    if(victim_hit == 0){
      // if there is no hit in victim cache, replace the cache block inside main cache
      if(is_write)  g_total_dm_victim_store_miss++;
      else  g_total_dm_victim_load_miss++;  
    }
    else{
      // if it hit in victim cache, need to move cache inside victim back to main cache
      if(is_write)  g_total_dm_victim_store_hit++;
      else  g_total_dm_victim_load_hit++; 
    } 
    // total miss = g_total_dm_store_miss + g_total_dm_load_miss
    // total store miss with victim = g_toal_dm_store_miss - g_total_dm_victim_store_hit
    // total load miss with victim = g_toal_dm_load_miss - g_total_dm_victim_load_hit
    dm_cache->tag[index] = tag;
    dm_cache->valid[index] = 1;
  }
}

/***** sa2w cache mapping *****/
void
sa2w_cache_map (struct sa2w_cache_t * sa2w_cache, struct victim_cache_t * victim_cache, int mem_addr, int is_write){
  int tag = mem_addr / 64 / 256;
  int index = mem_addr / 64 % 256;
  int offset = mem_addr % 64;
  int i;
  int tag_found = 0;
  
  for(i=0; i<2; i++){
    if(sa2w_cache->tag[i][index] == tag){
      tag_found = 1;
      if(sa2w_cache->valid[i][index] == 1){
        if(is_write)  g_total_sa2w_store_hit++;
        else  g_total_sa2w_load_hit++;  
      }
      else{
        // rewrite data
        if(is_write)  g_total_sa2w_store_miss++;
        else  g_total_sa2w_load_miss++; 
        sa2w_cache->valid[i][index] = 1;
      }
      // get the index of the matched set inside LRU 
      int j;
      int index_MRU;
      for(j = 0; j < 2 ; j++){
        if(sa2w_cache->LRU[index][j] == i)
          index_MRU = j;
      }

      // put the matched set at the start of the LRU array (most recent used) 
      int k;
      for (k=index_MRU; k>0; k--){
        sa2w_cache->LRU[index][k] = sa2w_cache->LRU[index][k-1];
      }
      sa2w_cache->LRU[index][0] = i;  // i is the most recent used
      break;
    }
  }

  if(tag_found == 0){
    // no tag found, call victim cache
    int replaced_index = sa2w_cache->LRU[index][2];
    int victim_hit = victimcache_map(victim_cache, (mem_addr/64), (sa2w_cache->tag[replaced_index][index]*256 + index));
    if(victim_hit == 0){
      // if there is no hit in victim cache, replace the cache block inside main cache
      if(is_write)  g_total_sa2w_store_miss++;
      else  g_total_sa2w_load_miss++;  
    }
    else{
      // if it hit in victim cache, need to move cache inside victim back to main cache
      if(is_write)  g_total_sa2w_store_hit++;
      else  g_total_sa2w_load_hit++; 
    } 

    // replace tag and update valid 
    sa2w_cache->tag[replaced_index][index] = tag;
    sa2w_cache->valid[replaced_index][index] = 1;
  }
}


/***** sa4w cache mapping *****/
void
sa4w_cache_map (struct sa4w_cache_t * sa4w_cache,  struct victim_cache_t * victim_cache, int mem_addr, int is_write){
  // calculating index, tag, offset by the memory address
  // since we need 8 bits index for 256 rows, and 5 bits offset for 32B data
  // so tag size = 32 - 8 - 5 = 19
  int tag = mem_addr / 32 / 256;
  int index = mem_addr / 32 % 256;
  int offset = mem_addr % 32;
  int tag_found = 0;
  int i;

  for(i=0; i<4; i++){
    if(sa4w_cache->tag[i][index] == tag){
      tag_found = 1;
      if(sa4w_cache->valid[i][index] == 1){
        if(is_write)  g_total_sa4w_store_hit++;
        else  g_total_sa4w_load_hit++; 
      }
      else{
        // rewrite data
        if(is_write)  g_total_sa4w_store_miss++;
        else  g_total_sa4w_load_miss++;
        sa4w_cache->valid[i][index] = 1;
      }
      // get the index of the matched set inside LRU 
      int j;
      int index_MRU;
      for(j = 0; j < 4 ; j++){
        if(sa4w_cache->LRU[index][j] == i)
          index_MRU = j;
      }

      // put the matched set at the start of the LRU array (most recent used) 
      int k;
      for (k=index_MRU; k>0; k--){
        sa4w_cache->LRU[index][k] = sa4w_cache->LRU[index][k-1];
      }
      sa4w_cache->LRU[index][0] = i;  // i is the most recent used
      break;
    }
  }

  if(tag_found == 0){
    int replaced_index = sa4w_cache->LRU[index][3];
    int victim_hit = victimcache_map(victim_cache, (mem_addr/64), (sa4w_cache->tag[replaced_index][index]*256 + index));
    if(victim_hit == 0){
      // if there is no hit in victim cache, replace the cache block inside main cache
      if(is_write)  g_total_sa4w_store_miss++;
      else  g_total_sa4w_load_miss++;  
    }
    else{
      // if it hit in victim cache, need to move cache inside victim back to main cache
      if(is_write)  g_total_sa4w_store_hit++;
      else  g_total_sa4w_load_hit++; 
    } 
        // replace tag and update valid 
    sa4w_cache->tag[replaced_index][index] = tag;
    sa4w_cache->valid[replaced_index][index] = 1;
  }
}

void 
sa8w_wb_cache_map (struct sa8w_wb_cache_t * sa8w_wb_cache, struct victim_cache_t * victim_cache4, int mem_addr, int is_write){
  // calculating index, tag, offset by the memory address
  // since we need 6 bits index for 64 rows, and 6 bits offset for 64B data
  // so tag size = 32 - 6 - 6 = 20
  int tag = mem_addr / 64 / 64;
  int index = mem_addr / 64 % 64;
  int offset = mem_addr % 64;
  int tag_found = 0;
  int i;
  //int LRU [][] = ;

  for(i = 0; i<8; i++){
    if(sa8w_wb_cache->tag[i][index] == tag){
      tag_found = 1;
      if(sa8w_wb_cache->valid[i][index] == 1){
        if(is_write){
          // if tag matched, data valid, and it is a write instruction, change dirty bit to 1
          sa8w_wb_cache->dirty[i][index] = 1;
          g_total_hit_store_sa8w_wb_cache++;
        }
        else{
          // hit, increament counter 
          g_total_hit_load_sa48_wb_cache++;
        }
      }
      else{
        // tag matched, data not valid, it is a miss
        if(is_write){
          g_total_miss_store_sa8w_wb_cache++;
          sa8w_wb_cache->dirty[i][index] = 1;
        }
        else{
          g_total_miss_load_sa8w_wb_cache++;
        }
        sa8w_wb_cache->valid[i][index] = 1;
      }
      
      // get the index of the matched set inside LRU 
      int j;
      int index_MRU;
      for(j = 0; j < 8 ;j++){
        if(sa8w_wb_cache->LRU[index][j] == i)
          index_MRU = j;
      }

      // put the matched set at the start of the LRU array (most recent used) 
      int k;
      for (k=index_MRU; k>0; k--){
        sa8w_wb_cache->LRU[index][k] = sa8w_wb_cache->LRU[index][k-1];
      }
      sa8w_wb_cache->LRU[index][0] = i;  // i is the most recent used
      break;
    }
  }

  if(tag_found == 0){
    int replaced_index = sa8w_wb_cache->LRU[index][7];

    // int victim_hit = victimcache_map(victim_cache4, (mem_addr/64), (sa8w_wb_cache->tag[replaced_index][index]*64 + index));
    // //int victim_hit = 0;
    // if(victim_hit == 0){
    //   // if there is no hit in victim cache, replace the cache block inside main cache
    //   if(is_write)  g_total_sa8w_vic_store_miss++;
    //   else  g_total_sa8w_vic_load_miss++;  
    // }
    // else{
    //   // if it hit in victim cache, need to move cache inside victim back to main cache
    //   if(is_write)  g_total_sa8w_vic_store_hit++;
    //   else  g_total_sa8w_vic_load_hit++; 
    // } 

    if(sa8w_wb_cache->dirty[replaced_index][index] == 1)
      // missed and dirty bit is 1, wtrite back to mem and increament write back counter
      g_total_write_back_event++;
    
    if(is_write){
      // store instruction, set dirty to 1, increament store miss count
      sa8w_wb_cache->dirty[replaced_index][index] = 1;
      g_total_miss_store_sa8w_wb_cache++;
    }
    else{
      // load instruction, set dirty to 0, increament load miss count
      sa8w_wb_cache->dirty[replaced_index][index] = 0;
      g_total_miss_load_sa8w_wb_cache++;
    }
    // replace tag and update valid 
    sa8w_wb_cache->tag[replaced_index][index] = tag;
    sa8w_wb_cache->valid[replaced_index][index] = 1;
  }

}



/***** cache implementation *****/

/* register simulator-specific options */
void
sim_reg_options(struct opt_odb_t *odb)
{
  opt_reg_header(odb, 
"sim-safe: This simulator implements a functional simulator.  This\n"
"functional simulator is the simplest, most user-friendly simulator in the\n"
"simplescalar tool set.  Unlike sim-fast, this functional simulator checks\n"
"for all instruction errors, and the implementation is crafted for clarity\n"
"rather than speed.\n"
		 );

  /* instruction limit */
  opt_reg_uint(odb, "-max:inst", "maximum number of inst's to execute",
	       &max_insts, /* default */0,
	       /* print */TRUE, /* format */NULL);

}

/* check simulator-specific option values */
void
sim_check_options(struct opt_odb_t *odb, int argc, char **argv)
{
  /* nada */
}

/* register simulator-specific statistics */
void
sim_reg_stats(struct stat_sdb_t *sdb)
{
  stat_reg_counter(sdb, "sim_num_insn",
		   "total number of instructions executed",
		   &sim_num_insn, sim_num_insn, NULL);
  stat_reg_counter(sdb, "sim_num_refs",
		   "total number of loads and stores executed",
		   &sim_num_refs, 0, NULL);
  stat_reg_int(sdb, "sim_elapsed_time",
	       "total simulation time in seconds",
	       &sim_elapsed_time, 0, NULL);
  stat_reg_formula(sdb, "sim_inst_rate",
		   "simulation speed (in insts/sec)\n",
		   "sim_num_insn / sim_elapsed_time", NULL);

  /***** added counters *****/
  stat_reg_counter(sdb, "tot_store_inst" /* label for printing */,
                 "total store instructions" /* description */,
                 &g_total_store_inst /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_counter(sdb, "tot_load_inst" /* label for printing */,
                 "total load instructions\n" /* description */,
                 &g_total_load_inst /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);

  stat_reg_counter(sdb, "dm_store_miss" /* label for printing */,
                 "total store misses in dm cache" /* description */,
                 &g_total_dm_store_miss /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "dm_store_miss_rate",
		   "dm cache store miss rate",
		   "dm_store_miss / tot_store_inst", NULL);
  stat_reg_counter(sdb, "dm_vic_store_hit" /* label for printing */,
                 "store hit in dm victim cache" /* description */,
                 &g_total_dm_victim_store_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "dm_vic_store_miss_rate",
		   "dm victim cache store miss rate\n",
		   "(dm_store_miss - dm_vic_store_hit) / tot_store_inst", NULL);


  stat_reg_counter(sdb, "dm_load_miss" /* label for printing */,
                 "toal load misses in dm cache" /* description */,
                 &g_total_dm_load_miss /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "dm_load_miss_rate",
		   "dm cache load miss rate",
		   "(dm_load_miss) / tot_load_inst", NULL);
  stat_reg_counter(sdb, "dm_vic_load_hit" /* label for printing */,
                 "load hit in dm victim cache" /* description */,
                 &g_total_dm_victim_load_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "dm_vic_load_miss_rate",
		   "dm victim cache load miss rate\n",
		   "(dm_load_miss - dm_vic_load_hit) / tot_load_inst", NULL);


  stat_reg_formula(sdb, "dm_cache_miss_rate",
		   "dm cache miss rate",
		   "(dm_store_miss + dm_load_miss) / sim_num_refs", NULL);
  stat_reg_formula(sdb, "dm_vic_cache_miss_rate",
		   "dm victim cache miss rate\n\n",
		   "((dm_store_miss + dm_load_miss) - (dm_vic_store_hit + dm_vic_load_hit)) / sim_num_refs", NULL);


///////////
  stat_reg_counter(sdb, "sa2w_vic_load_hit" /* label for printing */,
                 "load hit in 2w sa vic $" /* description */,
                 &g_total_sa2w_load_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_counter(sdb, "sa2w_vic_load_miss" /* label for printing */,
                 "load miss in 2w sa vic $" /* description */,
                 &g_total_sa2w_load_miss /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "sa2w_vic_load_miss_rate",
		   "sa2w vic $ load miss rate\n",
		   "sa2w_vic_load_miss / (sa2w_vic_load_hit + sa2w_vic_load_miss)", NULL);

  stat_reg_counter(sdb, "sa2w_vic_store_hit" /* label for printing */,
                 "store hit in 2w sa vic $" /* description */,
                 &g_total_sa2w_store_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_counter(sdb, "sa2w_vic_store_miss" /* label for printing */,
                 "store miss in 2w sa vic $" /* description */,
                 &g_total_sa2w_store_miss /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "sa2w_vic_store_miss_rate",
		   "sa2w vic $ store miss rate\n",
		   "sa2w_vic_store_miss / (sa2w_vic_store_hit + sa2w_vic_store_miss)", NULL);

  stat_reg_formula(sdb, "sa2w_vic_miss_rate",
		   "sa2w vic $ miss rate\n",
		   "((sa2w_vic_store_miss + sa2w_vic_load_miss) / ((sa2w_vic_load_hit + sa2w_vic_load_miss) + (sa2w_vic_store_hit + sa2w_vic_store_miss)))", NULL);
/////////////

///////////
  stat_reg_counter(sdb, "sa4w_vic_load_hit" /* label for printing */,
                 "load hit in 2w sa vic $" /* description */,
                 &g_total_sa4w_load_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_counter(sdb, "sa4w_vic_load_miss" /* label for printing */,
                 "load miss in 2w sa vic $" /* description */,
                 &g_total_sa4w_load_miss /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "sa4w_vic_load_miss_rate",
		   "sa4w vic $ load miss rate\n",
		   "sa4w_vic_load_miss / (sa4w_vic_load_hit + sa4w_vic_load_miss)", NULL);

  stat_reg_counter(sdb, "sa4w_vic_store_hit" /* label for printing */,
                 "store hit in 2w sa vic $" /* description */,
                 &g_total_sa4w_store_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_counter(sdb, "sa4w_vic_store_miss" /* label for printing */,
                 "store miss in 2w sa vic $" /* description */,
                 &g_total_sa4w_store_miss /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "sa4w_vic_store_miss_rate",
		   "sa4w vic $ store miss rate\n",
		   "sa4w_vic_store_miss / (sa4w_vic_store_hit + sa4w_vic_store_miss)", NULL);

  stat_reg_formula(sdb, "sa4w_vic_miss_rate",
		   "sa4w vic $ miss rate\n",
		   "((sa4w_vic_store_miss + sa4w_vic_load_miss) / ((sa4w_vic_load_hit + sa4w_vic_load_miss) + (sa4w_vic_store_hit + sa4w_vic_store_miss)))", NULL);/////////////




  stat_reg_counter(sdb, "miss_store_sa8w_wb_$" /* label for printing */,
                 "store miss 8w sa $" /* description */,
                 &g_total_miss_store_sa8w_wb_cache /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "store miss rate 8 way sa $",
		   "store miss rate in 8 way sa $",
		   "miss_store_sa8w_wb_$ / tot_load_inst", NULL);
  stat_reg_counter(sdb, "miss_load_sa8w_wb_$" /* label for printing */,
                 "load miss 8w sa $" /* description */,
                 &g_total_miss_load_sa8w_wb_cache /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "load miss rate in 8 way sa $",
		   "load miss rate 8 way sa $\n",
		   "miss_load_sa8w_wb_$ / tot_load_inst", NULL);

  stat_reg_counter(sdb, "write_back_event" /* label for printing */,
                 "total write back" /* description */,
                 &g_total_write_back_event /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "write_back_rate",
		   "write back rate \n",
		   "write_back_event / tot_load_inst", NULL);

  stat_reg_counter(sdb, "sa8w_vic_load_hit" /* label for printing */,
                 "load hit in sa8w victim cache" /* description */,
                 &g_total_sa8w_vic_load_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "sa8w_vic_load_miss_rate",
		   "sa8w + victim $ load miss rate\n",
		   "(miss_load_sa8w_wb_$ - sa8w_vic_load_hit) / tot_load_inst", NULL);
  stat_reg_counter(sdb, "sa8w_vic_store_hit" /* label for printing */,
                 "store hit in sa8w victim cache" /* description */,
                 &g_total_sa8w_vic_store_hit /* pointer to the counter */, 
                 0 /* initial value for the counter */, NULL);
  stat_reg_formula(sdb, "sa8w_vic_store_miss_rate",
		   "sa8w + victim $ store miss rate\n",
		   "(miss_store_sa8w_wb_$ - sa8w_vic_load_hit) / tot_load_inst", NULL);
  
  /**************************/
  ld_reg_stats(sdb);
  mem_reg_stats(mem, sdb);
}

/* initialize the simulator */
void
sim_init(void)
{
  sim_num_refs = 0;

  /* allocate and initialize register file */
  regs_init(&regs);

  /* allocate and initialize memory space */
  mem = mem_create("mem");
  mem_init(mem);
}

/* load program into simulated state */
void
sim_load_prog(char *fname,		/* program to load */
	      int argc, char **argv,	/* program arguments */
	      char **envp)		/* program environment */
{
  /* load program text and data, set up environment, memory, and regs */
  ld_load_prog(fname, argc, argv, envp, &regs, mem, TRUE);
}

/* print simulator-specific configuration information */
void
sim_aux_config(FILE *stream)		/* output stream */
{
  /* nothing currently */
}

/* dump simulator-specific auxiliary simulator statistics */
void
sim_aux_stats(FILE *stream)		/* output stream */
{
  /* nada */
}

/* un-initialize simulator-specific state */
void
sim_uninit(void)
{
  /* nada */
}


/*
 * configure the execution engine
 */

/*
 * precise architected register accessors
 */

/* next program counter */
#define SET_NPC(EXPR)		(regs.regs_NPC = (EXPR))

/* current program counter */
#define CPC			(regs.regs_PC)

/* general purpose registers */
#define GPR(N)			(regs.regs_R[N])
#define SET_GPR(N,EXPR)		(regs.regs_R[N] = (EXPR))

#if defined(TARGET_PISA)

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_L(N)		(regs.regs_F.l[(N)])
#define SET_FPR_L(N,EXPR)	(regs.regs_F.l[(N)] = (EXPR))
#define FPR_F(N)		(regs.regs_F.f[(N)])
#define SET_FPR_F(N,EXPR)	(regs.regs_F.f[(N)] = (EXPR))
#define FPR_D(N)		(regs.regs_F.d[(N) >> 1])
#define SET_FPR_D(N,EXPR)	(regs.regs_F.d[(N) >> 1] = (EXPR))

/* miscellaneous register accessors */
#define SET_HI(EXPR)		(regs.regs_C.hi = (EXPR))
#define HI			(regs.regs_C.hi)
#define SET_LO(EXPR)		(regs.regs_C.lo = (EXPR))
#define LO			(regs.regs_C.lo)
#define FCC			(regs.regs_C.fcc)
#define SET_FCC(EXPR)		(regs.regs_C.fcc = (EXPR))

#elif defined(TARGET_ALPHA)

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_Q(N)		(regs.regs_F.q[N])
#define SET_FPR_Q(N,EXPR)	(regs.regs_F.q[N] = (EXPR))
#define FPR(N)			(regs.regs_F.d[(N)])
#define SET_FPR(N,EXPR)		(regs.regs_F.d[(N)] = (EXPR))

/* miscellaneous register accessors */
#define FPCR			(regs.regs_C.fpcr)
#define SET_FPCR(EXPR)		(regs.regs_C.fpcr = (EXPR))
#define UNIQ			(regs.regs_C.uniq)
#define SET_UNIQ(EXPR)		(regs.regs_C.uniq = (EXPR))

#else
#error No ISA target defined...
#endif

/* precise architected memory state accessor macros */
#define READ_BYTE(SRC, FAULT)						\
  ((FAULT) = md_fault_none, addr = (SRC), MEM_READ_BYTE(mem, addr))
#define READ_HALF(SRC, FAULT)						\
  ((FAULT) = md_fault_none, addr = (SRC), MEM_READ_HALF(mem, addr))
#define READ_WORD(SRC, FAULT)						\
  ((FAULT) = md_fault_none, addr = (SRC), MEM_READ_WORD(mem, addr))
#ifdef HOST_HAS_QWORD
#define READ_QWORD(SRC, FAULT)						\
  ((FAULT) = md_fault_none, addr = (SRC), MEM_READ_QWORD(mem, addr))
#endif /* HOST_HAS_QWORD */

#define WRITE_BYTE(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, addr = (DST), MEM_WRITE_BYTE(mem, addr, (SRC)))
#define WRITE_HALF(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, addr = (DST), MEM_WRITE_HALF(mem, addr, (SRC)))
#define WRITE_WORD(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, addr = (DST), MEM_WRITE_WORD(mem, addr, (SRC)))
#ifdef HOST_HAS_QWORD
#define WRITE_QWORD(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, addr = (DST), MEM_WRITE_QWORD(mem, addr, (SRC)))
#endif /* HOST_HAS_QWORD */

/* system call handler macro */
#define SYSCALL(INST)	sys_syscall(&regs, mem_access, mem, INST, TRUE)

#define DNA         (0)

/* general register dependence decoders */
#define DGPR(N)         (N)
#define DGPR_D(N)       ((N) &~1)

/* floating point register dependence decoders */
#define DFPR_L(N)       (((N)+32)&~1)
#define DFPR_F(N)       (((N)+32)&~1)
#define DFPR_D(N)       (((N)+32)&~1)

/* miscellaneous register dependence decoders */
#define DHI         (0+32+32)
#define DLO         (1+32+32)
#define DFCC            (2+32+32)
#define DTMP            (3+32+32)

/* start simulation, program loaded, processor precise state initialized */
void
sim_main(void)
{
  md_inst_t inst;
  register md_addr_t addr;
  enum md_opcode op;
  register int is_write;
  //register int is_load;
  enum md_fault_type fault;

  fprintf(stderr, "sim: ** starting functional simulation **\n");

  /* set up initial default next PC */
  regs.regs_NPC = regs.regs_PC + sizeof(md_inst_t);
  
  struct dm_cache_t dm_cache;
  struct sa4w_cache_t sa4w_cache;
  struct sa8w_wb_cache_t sa8w_wb_cache;
  struct victim_cache_t victim_cache1;
  struct victim_cache_t victim_cache2;
  struct victim_cache_t victim_cache3;
  struct victim_cache_t victim_cache4;
  struct sa2w_cache_t sa2w_cache;
  
  // initializing array
  int i;
  int j;
  for(i = 0; i<256; i++){         // initializing LRU for 2 way
    // LRU initialzing
    for(j=0; j<2;j++){
      sa4w_cache.LRU[i][j] = 1-j;
    }
  }
  for(i = 0; i<256; i++){         // initializing LRU for 4 way
    // LRU initialzing
    for(j=0; j<4;j++){
      sa4w_cache.LRU[i][j] = 3-j;
    }
  }
  for(i = 0; i<64; i++){          // initializing LRU for 8 way
    // LRU initialzing
    for(j=0; j<8;j++){
      sa8w_wb_cache.LRU[i][j] = 7-j;
    }
  }

  while (TRUE)
    {
      /* maintain $r0 semantics */
      regs.regs_R[MD_REG_ZERO] = 0;
#ifdef TARGET_ALPHA
      regs.regs_F.d[MD_REG_ZERO] = 0.0;
#endif /* TARGET_ALPHA */

      /* get the next instruction to execute */
      // the inst directly get written to the mem.
      // can implement cache in here, 
      // check how the mem is initialized 
<<<<<<< HEAD
      
      if(is_write)  g_total_store_inst++;
      else          g_total_load_inst++;

      if (addr != 0){
        dm_cache_map(&dm_cache, &victim_cache1, addr, is_write);
        sa2w_cache_map(&sa2w_cache, &victim_cache2, addr, is_write);
        sa4w_cache_map (&sa4w_cache, &victim_cache3, addr, is_write);
        sa8w_wb_cache_map(&sa8w_wb_cache, &victim_cache4 ,addr, is_write);
      }

=======
      printf("%d", &mem);
>>>>>>> 98fd45ce6417201e676d9a64ddff2a34886fc48a
      MD_FETCH_INST(inst, mem, regs.regs_PC);


      /* keep an instruction count */
      sim_num_insn++;

      /* set default reference address and access mode */
      addr = 0; is_write = FALSE;

      /* set default fault - none */
      fault = md_fault_none;

      /* decode the instruction */
      // where the L1 cache can be implemented
      MD_SET_OPCODE(op, inst);

      /* execute the instruction */
      switch (op)
	{   
#define DEFINST(OP,MSK,NAME,OPFORM,RES,FLAGS,O1,O2,I1,I2,I3)		\
	case OP:							\
          SYMCAT(OP,_IMPL);						\
          break;
#define DEFLINK(OP,MSK,NAME,MASK,SHIFT)					\
        case OP:							\
          panic("attempted to execute a linking opcode");
#define CONNECT(OP)
#define DECLARE_FAULT(FAULT)						\
	  { fault = (FAULT); break; }
#include "machine.def"
	default:
	  panic("attempted to execute a bogus opcode");
      }

      if (fault != md_fault_none)
	fatal("fault (%d) detected @ 0x%08p", fault, regs.regs_PC);

      if (verbose)
	{
	  myfprintf(stderr, "%10n [xor: 0x%08x] @ 0x%08p: ",
		    sim_num_insn, md_xor_regs(&regs), regs.regs_PC);
	  md_print_insn(inst, regs.regs_PC, stderr);
	  if (MD_OP_FLAGS(op) & F_MEM)
	    myfprintf(stderr, "  mem: 0x%08p", addr);
	  fprintf(stderr, "\n");
	  /* fflush(stderr); */
	}

      if (MD_OP_FLAGS(op) & F_MEM)
	{
	  sim_num_refs++;
	  if (MD_OP_FLAGS(op) & F_STORE)
	    is_write = TRUE;
	}


      /* go to the next instruction */
      regs.regs_PC = regs.regs_NPC;
      regs.regs_NPC += sizeof(md_inst_t);

      /* finish early? */
      if (max_insts && sim_num_insn >= max_insts)
	return;
    }
}
