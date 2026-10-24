(defn m-power-set [s]
  (reduce
   (fn [acc val]
     (into acc (for [ss acc] (conj ss val))))
   #{#{}}
   s))

(= (m-power-set #{1 :a}) #{#{1 :a} #{:a} #{} #{1}})

(= (m-power-set #{}) #{#{}})

(= (m-power-set #{1 2 3})
   #{#{} #{1} #{2} #{3} #{1 2} #{1 3} #{2 3} #{1 2 3}})

(= (count (m-power-set (into #{} (range 10)))) 1024)
