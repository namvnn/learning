(defn m-split-at [n coll]
  (concat
   (list (take n coll))
   (list (take-last (- (count coll) n) coll))))

(= (m-split-at 3 [1 2 3 4 5 6]) [[1 2 3] [4 5 6]])

(= (m-split-at 1 [:a :b :c :d]) [[:a] [:b :c :d]])

(= (m-split-at 2 [[1 2] [3 4] [5 6]]) [[[1 2] [3 4]] [[5 6]]])
