# 1 - Architecture of paginated memory (simplified / segmentated memory)
- Physical memory is paginated, structured in blocks of 4 kilo bytes (usually)
- Each process has it's virtual paginated address space, structured in blocks of 4 kilo bytes (usually)
- The OS uses a schema of pagination, where for each process, they have a table that maps virtual addresses to physical ones. The formation of this association is a privileged action and is done by the kernel. We can call this the **table of pages**
# Example
- Let's imagine we are in a 32bit system
- 4 kilo bytes = 4096 bytes, which can be covered by 12 positions of bits = 2^12. So these bits are used used as the **offset** to identify a position of memory starting at a certain physical address indicated by the **table of pages**, and 4095 is it's maximum value, because we don't use offset==0
- 32-12 = 20. With these remaining 2^20 positions, we can identify a page number. This is the number (address) used to go through the table that associates virtual addresses to physical address
- After the kernel obtains the physical address, it uses the **offset** to calculate the actual & final address of memory

## Retrospective
- So in reality, before a process accesses a variable in physical memory (RAM), it first consults a part of the memory (RAM) to obtain a variable in memory

## Translation lookaside buffer
- Given this constant memory access, this buffer was exists in most OS in order to cache and thus speed up the memory accesses
- This was inspired by the fact of the principle of localization -> If I used an address, it's likely that I will use it again & If I used an address, it's likely that I will access a variable near to it. 
- It contains the cached & converted virtual addressed to physical addressed

# 2 - Exercise of dimension of pages & translations
## Given the environment
- Virtual & Physical addresses w/ 32bits (4GB)
- Table of address pages of 4 KiloBytes
- 12 extra bits are used for the configuration of each entry of the mapping in the table

## Calculate
### Number of virtual pages (available) for each address space (per process)
- 4 GigaBytes / 4 KiloBytes = 1 MegaBytes
- 1 MegaBytes = 1 000 KiloBytes = 1 000 000 bytes
- Thus, there can be around a million virtual pages per process

### Number of page frames in physical memory
- The size of page frames is equal to the size of virtual pages
- And each page frame is bound to an address in the virtual page
- So the number is equal to the previous answer

### Number of bits in each entry of a mapping table (addresses table)
- The entry of a table will be 32 bits = 4bytes. So each entry of the mapping table uses 4 bytes
- 32-12 = 20 bits, to identify a page
- The other 12 bits are used to index an address in physical memory

### Total size of a table mapping
- Since the number of entries (or virtual pages) is 1 MBy
- And each entry uses 4 bytes
- The total size of a table mapping (per process) is 1 MBy * 4 By = 4 MBy

### Total physical memory used by the mapping tables of 10 processes
- Since each process uses 4 Mby, 10 processes will ocupy 40 Mby.

# 3 - Tables of multi-level translation (non-segmented)
- To cut down the amount of memory used by mapping tables, this concept was introduced
- So now, processes can use a much more custom amount of memory
- Where the 20 bits of the page frame are divided into 2 parts of 10 bits. Where the bit-most significant part is the **page directory index** and the other is the **page table index**
- The **page directory index** will point to another table, of a 2nd level
- The **page table index** will point to the index of that 2nd table
- But these other tables are optional, meaning that processes that don't use many addresses, can now have their pages with smaller sizes
- So basically, all processes previously had to occupy 4 Mby. But now that a table entry can only use 10 bytes to reference a table, it can now point to an amount up to 1 KiloBytes (2^10) instead of 1 MegaBytes (2^20)

# 4 - Similar Exercise as *2* but w/ 2 translation levels
## Given the environment
- Virtual & Physical addresses w/ 32bits (4GB)
- Table of address pages of 4 KiloBytes
- 12 extra bits are used for the configuration of each entry of the mapping in the table
- **There are 2 levels of page translation, with 10 bits each**

### Total size of a table mapping
- Now that each table is indexed w/ 10 bits (not 20), each has 1024 (2^10) entries
- These entries have 4 bytes
- So each mapping-table has 4 Bytes * 1024 = 4 KiloBytes

### Min & Max size of the tables mapping of a process
- The Min will be 8 KiloBytes because there has to be 2 tables of 4 KiBy for the architecture to work. The Page Directory (pointed by the most significant 10 bits) and the 2nd Level Page Table (pointed by the least significant 10 bits)
- The Max will be all the 2nd level table w/ all the mapping in use 4 MegaBytes (1024*4KiBy), PLUS, the 1st level Table with 4KiloBytes -> 4096 KiBy (4MegBy) + 4 KiBy = 4100 KiBy,

### Min & Max size used in the physical memory by the tables mapping of 10 processes w/ 16 Kilobytes worth of virtual address spaces
- Min -> The 1st level table is of dimension 4KiBy, the 2nd level table is of dimension 4KiBy. And this table will have 4 page entries, and each of these should point to a table in physical memory of 4KiBy to make up for the 16 KiBy
- Max -> In the worst case, each of the 4 page entries would be in 4 different 2nd level tables. So we remain w/ 1 1st level table of 4KiBy, with 4 2nd level tables of 4 KiBy = 16 KiBy. The sum will be 20KiBy. So 10 processes will occupy 200KiBy

## Note about the error "Segmentation fault"
- This error is common in C/C++ programs and it's in fact an old message of the time where the architecture of segmentated memory was mostly used (although Linux always had paginated architecture)
- It actually means "memory address translation fault" or "invalid address"

# 5 - [Part2](./README2.md)
# References
- https://people.cs.rutgers.edu/~pxk/416/notes/10-paging.html
- https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/8_MainMemory.html
- https://www.gbmb.org/mb-to-kb
- https://cs.hac.ac.il/staff/martin/Micro_Modern/slide03.pdf (may not be very informative)

