### Exercise 1

**b) Subsections 1.1.3 and 5.3.1 specify a canonical address form for virtual addresses. With 4-level mapping, canonical addresses go from 0x0000000000000000 to 0x________________ and then from 0x________________ to 0xFFFFFFFFFFFFFFFF. Complete the missing values and write the canonical ranges for 5-level mapping.**

----

So that canonical form can exist, it's necessary to consider which bits in a address are the Signal Bits. For an address in canonical form to be considered valid, this bits (Signal), must all either be 0 or 1. The reason for such regards the transition from 32bits processors to 64bits, so that retrocompatability can occur, and so that the bits that shouldn't be accessed stay that way (CR4 can be onsidered for the example).

(Referências:  AMD64 Architecture Programmer’s Manual, Volume 2: System Programming, páginas pdf 65 e 201)

**4-level mapping**

In the case of 4-level mapping, the address Signal Bits are between the range 63...48.

```
0x0000000000000000 to 0x00007FFFFFFFFFFF
and
0xFFFF800000000000 to 0xFFFFFFFFFFFFFFFF
```

**5-level mapping**

In the case of 5-level mapping, the address Signal Bits are between the range 63...57.

```
0x0000000000000000 to 0x00FFFFFFFFFFFFFF
and
0xFF00000000000000 to 0xFFFFFFFFFFFFFFFF
```
