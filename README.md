
# skinny-on-avr

AVR implementations of SKINNY-128-128. The project is compiled with Atmel Studio 6.2 on Windows. The fastest speed is 223cycles/byte, which is very close to the data given in [skinny implementation].


### Cipher State

The cipher state of SKINNY-128-128 is

![Cipher State](./pic/cipher-state.png?raw=true "cipher state")

, where each element is a byte.


### Encryption

![Encryption](./pic/encryption.png?raw=true "encryption")


[skinny implementation]:https://sites.google.com/site/skinnycipher/implementation
