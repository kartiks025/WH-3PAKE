Fixed and Terminology -
1. Group G is fixed
2. prime P is fixed
3. Omega(O) is the set of 52 upper case and lower case letters and 10 digits
4. On is the set of all strings of length n in O
5. n is fixed at 14
6. Phi is the captcha function from pypi library
7. Key derivation from Password - Use a hash function or Kartik's method from BTP
8. H -> A one way hash function - MD5

Trusted part(Server) S generates and publishes -
  a. Generator g of group G
  b. Symmetric Encryption Schemes 
    Ciphers -
    i. AES(Rijndael)
    ii. Twofish
    iii. Serpent
    Modes -
    i. Counter Mode
    ii. Output Feedback Mode
    iii. Cipher Block Chaining Mode
  c. Hash Function
    i. MD4
    ii. MD5
    
Steps -

1. A -> A chooses random integer x from Zp* - Look for a good random number generator
2. A -> Compute M1 = E_{PWa}(g^x) - Use specified encryption scheme from gcrypt
3. A -> A sends M1 to B along with identities of A and B
4. B -> On getting M1, B chooses y from Zp* - Same RNG as Step 1
5. B -> Compute M2 = E_{PWa}(g^x) - Use specified encryption scheme from gcrypt
6. B -> B sends M1 and M2 to S with identities of A and B
7. S -> S decrypts M1 and M2 to retrieve g^x and g^y
8. S -> Pick s1 and s2 at random from Zp* - Same RNG as Step 1
9. S -> Compute Kas = g^(x*s1) and Kbs = g^(y*s2)
10. S -> Pick r at random from On
11. S -> Pick t1 and t2 randomly from an arbitrary long enough set of integers - let's say Z{-1000-1000}
12. S -> For Captcha 
  a. Pypi library - https://pypi.org/project/captcha/
  b. For embedding refer this - https://docs.python.org/2/extending/embedding.html
13. S -> S computes M3 = E_{Kbs}(phi(r,t1)), M4 = E_{PWb}(g^s2), M5 = E_{Kas}(phi(r,t2)), M6 = E_{PWa}(g^s1)
14. S -> S sends M3, M4, M5, M6 to B
15. B -> B decrypts M4 to retrieve g^s2
16. B -> B computes g^(s2*y)
17. B -> B gets Pic1 = Phi(r,t1) by decrypting M3 using g^(s2*y)
18. B -> Manually solve captcha Pic1 - if no recognizable string r - terminate.
19. B -> B computes M7 = H(1||r||B||A)
20. B -> B sends M5, M6, M7 to A
21. A -> Decrypt M6, and computes g^(s1*x)
22. A -> A gets Pic2 = Phi(r,t2) by decrypting M5 using g^(s1*x)
23. A -> Manually solve captcha Pic2 - if no recognizable string r - terminate.
24. A -> If r is there, verify M7, if fail terminate.
25. A -> Compute M8 = H(1||r||A||B) and sk = H(2||r||A||B)
26. A -> Send M8 to B
27. B -> Verify M8
28. B -> Compute sk = H(2||r||A||B) and over.
