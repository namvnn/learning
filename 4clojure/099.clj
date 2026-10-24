(defn m-product-digits [x y]
  (->> (* x y)
       (str)
       (map (comp read-string str))))

(= (m-product-digits 1 1) [1])

(= (m-product-digits 99 9) [8 9 1])

(= (m-product-digits 999 99) [9 8 9 0 1])
