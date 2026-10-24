(defn m-cartesian-product [s1 s2]
  (set (for [x s1 y s2] [x y])))

(= (m-cartesian-product #{"ace" "king" "queen"} #{"♠" "♥" "♦" "♣"})
   #{["ace" "♠"] ["ace" "♥"] ["ace" "♦"] ["ace" "♣"]
     ["king" "♠"] ["king" "♥"] ["king" "♦"] ["king" "♣"]
     ["queen" "♠"] ["queen" "♥"] ["queen" "♦"] ["queen" "♣"]})

(= (m-cartesian-product #{1 2 3} #{4 5})
   #{[1 4] [2 4] [3 4] [1 5] [2 5] [3 5]})

(= 300 (count (m-cartesian-product (into #{} (range 10))
                                   (into #{} (range 30)))))
