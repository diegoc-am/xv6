#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

#define MAXELEM 512

struct shmem{
	char *mem;
	uint proc_count;
};

struct shmem arrayMem[MAXELEM];

//hash
uint hash(uint a,uint size)
{
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a%size;
}


int sys_shmem(void){
	uint shmid;
	int key;
	
	if(argint(0, &key) < 0)
    return -1;
  
  //Generate the shmid
  shmid = hash(key,MAXELEM);
  //Check if exist
  if(arrayMem[shmid].mem != 0){
  	return shmid;
  }
  
  if((arrayMem[shmid].mem = kalloc())==0)
  	return -1;
  //Clean the page
  memset(arrayMem[shmid].mem, 0, PGSIZE);  
  return shmid;	
}

int sys_shmctl(void){
	int shmid;
	if(argint(0, &shmid) < 0)
    return -1;
  
	if(arrayMem[shmid].proc_count == 0){
		kfree(arrayMem[shmid].mem);
		arrayMem[shmid].mem = 0;
		return 1;
	}
	return 0;
}

extern int mappages(pde_t *, void *, uint , uint , int );


int sys_shmat(void){
	int shmid;
	uint addr;
	
	if(argint(0, &shmid) < 0)
    return 0;
   
  if(arrayMem[shmid].mem){
  	//Mapp the memory to the process by growing the size of the proc
  	addr = PGROUNDUP(proc->sz);
  	mappages(proc->pgdir, (char*)addr, PGSIZE, v2p(arrayMem[shmid].mem), PTE_W|PTE_U);
  	proc->sz += PGSIZE;
  	switchuvm(proc);
  	arrayMem[shmid].proc_count += 1;
  	return addr;
  }
  else
  	return 0;
}

extern pte_t * walkpgdir(pde_t *, const void *, int );
int sys_shdemat(void){
  int shmid;
	uint addr;
	pte_t *pte;
	
	
	if(argint(0, &shmid) < 0)
    return 0;
  
  if(arrayMem[shmid].mem){
  	//Deattach the memory
  	addr = PGROUNDUP(proc->sz - PGSIZE);
  	pte = walkpgdir(proc->pgdir, (char*)addr, 0);
  	*pte = 0;
  	arrayMem[shmid].proc_count -= 1;
  	proc->sz -= PGSIZE;
  	return 1;
  }
  else
  	return 0;
}

	

int sys_initshmem(void){
	int i;
	for(i=0;i<MAXELEM;i++){
		arrayMem[i].mem = 0;
		arrayMem[i].proc_count = 0;
	}
	return 0;
}
