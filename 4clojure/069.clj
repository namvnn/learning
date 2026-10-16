(defn m-merge-fn [f & coll]
  (reduce
   (fn [r m]
     (reduce
      (fn [r k]
        (if (contains? r k)
          (assoc r k (f (get r k) (get m k)))
          (assoc r k (get m k))))
      r
      (keys m)))
   (first coll)
   (rest coll)))

(= (m-merge-fn * {:a 2, :b 3, :c 4} {:a 2} {:b 2} {:c 5})
   {:a 4, :b 6, :c 20})

(= (m-merge-fn - {1 10, 2 20} {1 3, 2 10, 3 15})
   {1 7, 2 10, 3 15})

(= (m-merge-fn concat {:a [3], :b [6]} {:a [4 5], :c [8 9]} {:b [7]})
   {:a [3 4 5], :b [6 7], :c [8 9]})
