
# skinny-on-avr

AVR implementations of SKINNY-128-128. The project is compiled with Atmel Studio 6.2 on Windows.


### Cipher State

The cipher state of SKINNY-128-128 is
s0  s1  s2  s3
s4  s5  s6  s7
s8  s9  s10 s11
s12 s13 s14 s15
where each element is a byte.


### Encryption

                  s13 s14 s15 s12
 load plaintext   s0  s1  s2  s3
----------------> s7  s4  s5  s6  --------------
                  s10 s11 s8  s9               |
                                               |
  sub column, registers recover to right order |   -------
------------------------------------------------      ^
|                                                     |
|                 s0  s1  s2  s3                      |
|                 s4  s5  s6  s7                      |
----------------> s8  s9  s10 s11  -------------      |
                  s12 s13 s14 s15              |      |
                                               |      |
  add round const and round keys together      |
------------------------------------------------     one
|
|                 s0  s1  s2  s3                    round
|                 s4  s5  s6  s7
----------------> s8  s9  s10 s11  -------------      |
                  s12 s13 s14 s15              |      |
                                               |      |
  mix column and shift row, but the registers  |      |
------------------------------------------------      |
| are in wrong order.                          |      |
|                                                     |
|                 s13 s14 s15 s12                     |
|                 s0  s1  s2  s3                      |
----------------> s7  s4  s5  s6  --------------      |
                  s10 s11 s8  s9               |      V
                                               |   -------
  store cipher text                            |
<-----------------------------------------------