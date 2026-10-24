(defn m-pascal-triangle [n]
  (letfn [(pascal-triangle
           [i n]
           (if (or (= i 0) (= i (dec n)))
             1
             (+
              (pascal-triangle (dec i) (dec n))
              (pascal-triangle i (dec n)))))]
    (map #(pascal-triangle % n) (range 0 n))))

(= (m-pascal-triangle 1) [1])

(= (map m-pascal-triangle (range 1 6))
   [[1]
    [1 1]
    [1 2 1]
    [1 3 3 1]
    [1 4 6 4 1]])

(= (m-pascal-triangle 11)
   [1 10 45 120 210 252 210 120 45 10 1])

(= (m-pascal-triangle 11)
   [1 10 45 120 210 252 210 120 45 10 1])
