(defn m-tree-to-table [m]
  (->> (for [[k1 m1] m [k2 v2] m1] [[k1 k2] v2])
       (apply concat [])
       (apply hash-map)))

(= (m-tree-to-table '{a {p 1, q 2}
                      b {m 3, n 4}})
   '{[a p] 1, [a q] 2
     [b m] 3, [b n] 4})

(= (m-tree-to-table '{[1] {a b c d}
                      [2] {q r s t u v w x}})
   '{[[1] a] b, [[1] c] d,
     [[2] q] r, [[2] s] t,
     [[2] u] v, [[2] w] x})

(= (m-tree-to-table '{m {1 [a b c] 3 nil}})
   '{[m 1] [a b c], [m 3] nil})
