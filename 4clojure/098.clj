(defn m-equivalence-classes [f D]
  (->> D
       (group-by f)
       (vals)
       (map set)
       (set)))

(= (m-equivalence-classes #(* % %) #{-2 -1 0 1 2})
   #{#{0} #{1 -1} #{2 -2}})

(= (m-equivalence-classes #(rem % 3) #{0 1 2 3 4 5})
   #{#{0 3} #{1 4} #{2 5}})

(= (m-equivalence-classes identity #{0 1 2 3 4})
   #{#{0} #{1} #{2} #{3} #{4}})

(= (m-equivalence-classes (constantly true) #{0 1 2 3 4})
   #{#{0 1 2 3 4}})
