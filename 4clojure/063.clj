(defn m-group-by [f coll]
  (reduce
    (fn [m v] (update-in m [(f v)] concat [v]))
    {}
    coll))

(= (m-group-by #(> % 5) #{1 3 6 8}) {false [1 3], true [6 8]})

(= (m-group-by #(apply / %) [[1 2] [2 4] [4 6] [3 6]])
   {1/2 [[1 2] [2 4] [3 6]], 2/3 [[4 6]]})

(= (m-group-by count [[1] [1 2] [3] [1 2 3] [2 3]])
   {1 [[1] [3]], 2 [[1 2] [2 3]], 3 [[1 2 3]]})
