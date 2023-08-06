# stop checking the hash set for avoiding piece duplicates

 - create an array where for each piece I have the list of all the pieces that can form a connection with it  
this can be done either with a graph or a hash function... it remains to be seen witch one is better
 - IDEA: use a vectr of bit where each bit rappresent if the piece has or not the corrispondent piece in it and use that as an hash
 - circle through all the Permutation with No repetition for each group of pieces
 - use some sort of has for speed up the comparison for each piece