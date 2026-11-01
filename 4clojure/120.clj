(defn smaller-square? [coll]
  (letfn [(ss? [n]
               (let [x (map (comp read-string str) (str n))
                     s (reduce (fn [acc val] (+ acc (* val val))) 0 x)]
                 (< n s)))]
    (count (filter ss? coll))))

(= 8 (smaller-square? (range 10)))

(= 19 (smaller-square? (range 30)))

(= 50 (smaller-square? (range 100)))

(= 50 (smaller-square? (range 1000)))
