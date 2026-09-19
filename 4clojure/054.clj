(defn m-partition [n coll]
  (filter #(= (count %) n)
          (loop [c coll
                 par []]
            (if (empty? c)
              par
              (recur (drop n c) (conj par (take n c)))))))

(= (m-partition 3 (range 9)) '((0 1 2) (3 4 5) (6 7 8)))

(= (m-partition 2 (range 8)) '((0 1) (2 3) (4 5) (6 7)))

(= (m-partition 3 (range 8)) '((0 1 2) (3 4 5)))
