Indraneel Purohit and Shane O'Hanlon Indexer

One key decision made was to put a prefix trie at the center of this entire program.
Prefix tries are lexically based data structures, meaning that their structure is highly useful in the context of dealing with words, as we are here.
The obvious answer to this solution is a hash table, but that is kind of a pain to implement in C.
The trie has a few advantages over the hash table:
- faster lookup in the worst case (O(n) where n is the length of the string) vs a hash table with many collisions
- no collisions in a trie
- provides alphabetical ordering (this was important here)

There's disadvantages too
- can be slower than a hashtable (usually is)
- aren't necessarily more space efficient (though they usually are)

The rest of the algorithm is quite straightfoward.  
- Iterate over the m input files O(m)
- Tokenize each file of length n into q tokens O(n)
- For each token of length p, iterate over the token char by char O(q*p*n*m)
- As each char is received, traverse the trie based on the char O(q*p*n*m)
- At teh end of the token, demarcate that a word has been located and Search through, add or update the SortedList at each node 
- Inorder traversal of trie, saving each name => list pair into a file

This, obviously, takes linear time.

