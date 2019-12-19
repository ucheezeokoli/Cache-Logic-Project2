 #include "tips.h"
 #include <stdbool.h>

/* The following two functions are defined in util.c */

/* finds the highest 1 bit, and returns its position, else 0xFFFFFFFF */
unsigned int uint_log2(word w); 

/* return random int from 0..x-1 */
int randomint( int x );

/*
  This function allows the lfu information to be displayed

    assoc_index - the cache unit that contains the block to be modified
    block_index - the index of the block to be modified

  returns a string representation of the lfu information
 */
char* lfu_to_string(int assoc_index, int block_index)
{
  /* Buffer to print lfu information -- increase size as needed. */
  static char buffer[9];
  sprintf(buffer, "%u", cache[assoc_index].block[block_index].accessCount);

  return buffer;
}

/*
  This function allows the lru information to be displayed

    assoc_index - the cache unit that contains the block to be modified
    block_index - the index of the block to be modified

  returns a string representation of the lru information
 */
char* lru_to_string(int assoc_index, int block_index)
{
  /* Buffer to print lru information -- increase size as needed. */
  static char buffer[9];
  sprintf(buffer, "%u", cache[assoc_index].block[block_index].lru.value);

  return buffer;
}

/*
  This function initializes the lfu information

    assoc_index - the cache unit that contains the block to be modified
    block_number - the index of the block to be modified

*/
void init_lfu(int assoc_index, int block_index)
{
  cache[assoc_index].block[block_index].accessCount = 0;
}

/*
  This function initializes the lru information

    assoc_index - the cache unit that contains the block to be modified
    block_number - the index of the block to be modified

*/
void init_lru(int assoc_index, int block_index)
{
  cache[assoc_index].block[block_index].lru.value = 0;
}







// I IS THE COUNTER AND CONTAINS OFFSET VALUE
int getOffsetBits(address addr){

  int j = 1;
  int i;

  for (i = 0; j < block_size; i++){
    j *= 2;
  }

return i;
}

// I IS THE COUNTER AND CONTAINS INDEX VALUE
int getIndexBits(address addr){
  int j = 1;
  int i;

  for(i = 0; j < set_count; i++){
    j *= 2;
  }

return i;
}

// POWER FUNCTION

int Power (int a){
int temp =1;
if (a == 0){
	return 1;
}

else{
for(int i=a; i > 0; i--){
	 temp = 2 * temp;
}
}

	return temp;
}

// VALUE OF INDEX
int getIndexVal(address addr, int ind_b, int off){
  int c, k = 0, i = (ind_b - 1), val_i = 0;
  
 for (c = ((off -1) + ind_b); c >= off; c--){

    k = addr >> c;     
  
        if (k & 1) {
            val_i += Power(i);
            }
else {
//do nothing because the bit is 0
}
i--;
}
return val_i;
}

// VALUE OF OFFSET
int getOffsetVal(address addr, int off){
  int c, k = 0, i = (off - 1), val_o = 0;
  
 for (c = (off -1); c >= 0; c--){

    k = addr >> c;     
  
        if (k & 1) {
            val_o += Power(i);
            }
else {
//do nothing because the bit is 0
}
i--;
}
return val_o;
}

int getTagVal(address addr, int end){

  addr = addr >> end;

  int c;
  int tag = 0;
  int i = 31;

  for (c = 31; c >= 0; c--){
    int k = addr >> c;
    if(k & 1){
      tag += Power(i);
    }
    else{
      //DO NOTHING
    }
    i--;
  }
  return tag;
}

// int store [32];
// void getStore(int store[]){

// size_t str = sizeof(store);
// for (int i = 0; i < str; i++ ){
//     store [i] = 0;
//   }
// }

int count = 0;

bool check(int tag_v, int ind_v){
  bool  hit_miss = true;//MISS
  int i;
  // getStore(store);
    for(i = 0; i < assoc; i++){
    if(tag_v == cache[ind_v].block[i].tag){ 
        hit_miss = false;//HIT
        // store [0] = i;
        count = i;
        break;
    }
  }
  return hit_miss;
  }

  bool zero(int ind_v){
    bool zero = true;
      for(int i = 0; i < assoc; i++){
        if(cache[ind_v].block[i].lru.value != 0){
          zero = false;
        }
      }
    return zero;
  }

  bool one(int ind_v){
    bool zero = true;
      for(int i = 0; i < assoc; i++){
        if(cache[ind_v].block[i].lru.value != 1){
          zero = false;
        }
      }
    return zero;
  }
/*
  This is the primary function you are filling out,
  You are free to add helper functions if you need them

  @param addr 32-bit byte address
  @param data a pointer to a SINGLE word (32-bits of data)
  @param we   if we == READ, then data used to return
              information back to CPU

              if we == WRITE, then data used to
              update Cache/DRAM
*/
void accessMemory(address addr, word* data, WriteEnable we)
{
  /* Declare variables here */

unsigned int tag_v;
int end_of_addr;
CacheAction action;
int lru_i;
int min;

  /* handle the case of no cache at all - leave this in */
  if(assoc == 0) {
    accessDRAM(addr, (byte*)data, WORD_SIZE, we);
    return;
  }

  /*
  You need to read/write between memory (via the accessDRAM() function) and
  the cache (via the cache[] global structure defined in tips.h)

  Remember to read tips.h for all the global variables that tell you the
  cache parameters

  The same code should handle random, LFU, and LRU policies. Test the policy
  variable (see tips.h) to decide which policy to execute. The LRU policy
  should be written such that no two blocks (when their valid bit is VALID)
  will ever be a candidate for replacement. In the case of a tie in the
  least number of accesses for LFU, you use the LRU information to determine
  which block to replace.

  Your cache should be able to support write-through mode (any writes to
  the cache get immediately copied to main memory also) and write-back mode
  (and writes to the cache only gets copied to main memory when the block
  is kicked out of the cache.

  Also, cache should do allocate-on-write. This means, a write operation
  will bring in an entire block if the block is not already in the cache.

  To properly work with the GUI, the code needs to tell the GUI code
  when to redraw and when to flash things. Descriptions of the animation
  functions can be found in tips.h
  */

  /* Start adding code here */

int off_b = getOffsetBits(addr);

int off_v = getOffsetVal(addr,off_b);

int ind_b = getIndexBits(addr);

int ind_v = getIndexVal(addr, ind_b, off_b);

  end_of_addr = off_b + ind_b;


// tag_irrelevant = 32 - (end_of_addr);

tag_v = getTagVal(addr, end_of_addr);





//CORRECT TRANSFER SIZE FOR ACCESSDRAM
int transfer_size;

  switch(block_size)
    {
    case 1:
      transfer_size = BYTE_SIZE;
      break;
    case 2:
      transfer_size = HALF_WORD_SIZE;
      break;
    case 4:
      transfer_size = WORD_SIZE;
      break;
    case 8:
      transfer_size = DOUBLEWORD_SIZE;
      break;
    case 16:
      transfer_size = QUADWORD_SIZE;
      break;
    case 32:
      transfer_size = OCTWORD_SIZE;
      break;
    }


printf("ind_v: %d\n", ind_v);
printf("ind_b: %d\n", ind_b);
printf("off_v: %d\n", off_v);
printf("off_b: %d\n", off_b);
printf("tag_v: %8.8x\n", tag_v);


bool h_m = check(tag_v, ind_v);

// //_____INITIALZING THE LRU TO ZERO______//
// for (int i = 0; i < assoc; i++){
//       cache[ind_v].block[i].lru.value = 0;
//     }

//_______MISS_START________//
if (h_m == true){//MISS CASE
  action = MISS;
  if (policy == LRU){//REPLACEMENT POLICY
    min = cache[ind_v].block[0].lru.value;
      for(int i = 0; i < assoc; i++){
        if(cache[ind_v].block[i].lru.value < min){
            min = cache[ind_v].block[i].lru.value;
            lru_i = i;
        }
     }
      zero(ind_v);
      one(ind_v);
        if(zero(ind_v) == true){
          cache[ind_v].block[0].lru.value++;
          highlight_offset(ind_v, 0, off_v, action);
          highlight_block(ind_v, 0);
          cache[ind_v].block[0].tag = tag_v;
          cache[ind_v].block[0].valid = VALID;
          accessDRAM(addr, (cache[ind_v].block[0].data), transfer_size, READ);
          memcpy(data, (cache[ind_v].block[0].data + off_v), 4); //need 4 bytes each time
        }
       else if(one(ind_v) == true){
          for (int i = 0; i < assoc; i++){
            cache[ind_v].block[i].lru.value = 0;
          }
          cache[ind_v].block[0].lru.value = 1;
          highlight_offset(ind_v, 0, off_v, action);
          highlight_block(ind_v, 0);
          cache[ind_v].block[0].tag = tag_v;
          cache[ind_v].block[0].valid = VALID;
          accessDRAM(addr, (cache[ind_v].block[0].data), transfer_size, READ);
          memcpy(data, (cache[ind_v].block[0].data + off_v), 4); //need 4 bytes each time
        }
    else{
      cache[ind_v].block[lru_i].lru.value++;
      highlight_offset(ind_v, lru_i, off_v, action);
      highlight_block(ind_v, lru_i);
      cache[ind_v].block[lru_i].tag = tag_v;
      cache[ind_v].block[lru_i].valid = VALID;
      accessDRAM(addr, (cache[ind_v].block[lru_i].data), transfer_size, READ);
      memcpy(data, (cache[ind_v].block[lru_i].data + off_v), 4); //need 4 bytes each time
    }
  }

  if (policy == RANDOM){//REPLACEMENT POLICY
    lru_i = randomint(assoc);
    zero(ind_v);
        if(zero(ind_v) == true){
          cache[ind_v].block[0].lru.value++;
          highlight_offset(ind_v, 0, off_v, action);
          highlight_block(ind_v, 0);
          cache[ind_v].block[0].tag = tag_v;
          cache[ind_v].block[0].valid = VALID;
          accessDRAM(addr, (cache[ind_v].block[0].data), transfer_size, READ);
          memcpy(data, (cache[ind_v].block[0].data + off_v), 4); //need 4 bytes each time
        }
    else{
      cache[ind_v].block[lru_i].lru.value++;
      highlight_offset(ind_v, lru_i, off_v, action);
      highlight_block(ind_v, lru_i);
      cache[ind_v].block[lru_i].tag = tag_v;
      cache[ind_v].block[lru_i].valid = VALID;
      accessDRAM(addr, (cache[ind_v].block[lru_i].data), transfer_size, READ);
      memcpy(data, (cache[ind_v].block[lru_i].data + off_v), 4); //need 4 bytes each time
    }
  }
}
//________END OF MISS__________//


//________HIT_START___________//
if (h_m == false){
  action = HIT;
  highlight_offset(ind_v,count, off_v, action);
  memcpy(data, (cache[ind_v].block[count].data + off_v),4);
}
//_______HIT_END______________//








// // ________READ_START__________//
// if (we == READ){


// if (policy == LRU){
//     for (int i = 0; i < assoc; i++){
// if(tag_v != cache[ind_v].block[i].tag){ //cache miss
//     action = MISS;
//     highlight_offset(ind_v, i, off_v, action);
//     highlight_block(ind_v, i);
//     cache[ind_v].block[i].tag = tag_v;

   
    
    

//     cache[ind_v].block[i].valid = VALID;
//     accessDRAM(addr, (cache[ind_v].block[i].data), transfer_size, READ);
    
//     memcpy(data, (cache[ind_v].block[i].data + off_v), 4); //need 4 bytes each time
//    // printf("%8.8u\n", *cache[ind_v].block[0].data + off_v);
// }
//     }
//   }

//{
  //CACHE MISS
  // if (tag_v != cache[ind_v].block[0].tag /*&& cache[ind_v].block[0].valid != 1*/){
  //   action = MISS;

  //   highlight_offset(ind_v,0, off_v, action);
  //   highlight_block(ind_v,0);

  //   cache[ind_v].block[0].valid = VALID;

  //   cache[ind_v].block[0].tag = tag_v;
  //   memcpy(data, (cache[ind_v].block[0].data + off_v),4);
  //   accessDRAM(addr, (cache[ind_v].block[0].data), transfer_size, READ);
  // }
//   else{
//     action = HIT;
//     // cache[ind_v].block[0].dirty = DIRTY;
//     cache[ind_v].block[0].lru.value = 0;
//     highlight_offset(ind_v,0, off_v, action);
//     memcpy(data, (cache[ind_v].block[0].data + off_v),4);

//   }
// }
// ________READ_END__________//


  /* This call to accessDRAM occurs when you modify any of the
     cache parameters. It is provided as a stop gap solution.
     At some point, ONCE YOU HAVE MORE OF YOUR CACHELOGIC IN PLACE,
     THIS LINE SHOULD BE REMOVED.
  */
  

}
