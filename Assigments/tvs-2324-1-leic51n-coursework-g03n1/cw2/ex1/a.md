### Exercise 1

**a) Compare Figure 5-17 with Figure 5-18. The change from 4-level mapping to 5-level mapping increases the maximum size of virtual address spaces. Calculate the maximum size for each case.**

----

**4-level mapping**

Bits per level:
```
    - Physical-Page         : 12
    - Page-Table            : 9
    - Page-Directory        : 9
    - Page-Directory Pointer: 9
    - Page-Map Level-4      : 9
```
    

Total Bits = 2^12 * 2^9 * 2^9 * 2^9 * 2^9 = 2^48.

```
2^48 = 2^8 * 2^10 * 2^10 * 2^10 * 2^10 = 256TB.

              T      G      M      K
```

Answer: 256TB of maximum space for virtual space.

**5-level mapping**

Bits per level:
```
    - Physical-Page         : 12
    - Page-Table            : 9
    - Page-Directory        : 9
    - Page-Directory Pointer: 9
    - Page-Map Level-4      : 9
    - Page-Map Level-5      : 9
```
    
Total Bits = 2^12 * 2^9 * 2^9 * 2^9 * 2^9 * 2^9 = 2^57.

```
2^57 = 2^7 * 2^10 * 2^10 * 2^10 * 2^10 * 2^10 = 128PB.

              P      T      G      M      K
```

Resposta: 128PB of maximum space for virtual space.
