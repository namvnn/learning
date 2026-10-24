(defn m-partially-flatten-seq [coll]
  (mapcat
   (fn [v]
     (if (coll? (first v))
       (m-partially-flatten-seq v)
       [v]))
   coll))

(= (m-partially-flatten-seq [["Do"] ["Nothing"]])
   [["Do"] ["Nothing"]])

(= (m-partially-flatten-seq [[[[:a :b]]] [[:c :d]] [:e :f]])
   [[:a :b] [:c :d] [:e :f]])

(= (m-partially-flatten-seq '((1 2) ((3 4) ((((5 6)))))))
   '((1 2) (3 4) (5 6)))
