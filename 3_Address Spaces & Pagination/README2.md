# 1 - Page Table Entry (old i386 CPU)
![](../_imgs/PTE%20(OG)gif)
## The 3 rightmost bits are important
- P -> Present -> Indicates if this PTE is valid & is associated to a page frame. If it's 0, any attempt to translate into this page, an exception occurs
- R/W -> Read/Write Indicates if this PTE can be modified
- S/U -> Supervisor/User -> Indicates if this PTE is accessible by only the supervisor or the user (and supervisor)
## The other are use as informational for external sources
- A -> Accessed -> The page was accessed
- D -> Dirty -> The page was modified
- These bits are altered by the Memory Management Unit

# 2 - Exercise 2 about pages
## Consider:
- Virtual addresses w/ 36 bits
- The pagination structure has 2 levels:
    - 1st level -> 10 bits for the page directory table
    - 2st level -> 12 bits to index the page tables

## Calculate
### The size of the tables (all) and the size of each PTE
- We have 1024 (2^10) tables of 2nd level, which the 1st level table can point to
- And each 2nd level table, has 2^12 entries (addresses) = 4096
- 36 (total bits) - 22 (translation bits) = 14 offset bits = size of physical pages = 16 KiBy 
- Teacher says: Now, each PTE that's in the 2nd level tables will need to be able to reference the positions of each physical table which are of 16KiBy. 16KiBy / 4096 PTE's = 4 bytes
- My rationale says: It's fewer than 32 bits or fewer https://stackoverflow.com/a/52248364. See [PTE img](../_imgs/PTE%20(OG)gif)

### How many pages can exist in the virtual address space?
- 2^22 = 4 194 304 = 4Megas
- (And each one has 16 KiBy)

### W/ 21 bits in the PTE to indicate the page frame, whats the space of the physical address?
- 21 + 14 offset bits = 35 bits -> 2^35

# References
- https://stackoverflow.com/questions/23017488/what-is-page-table-entry-size
