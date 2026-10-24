(defn m-binary-tree? [coll]
  (and
   (coll? coll)
   (= (count coll) 3)
   (not (coll? (nth coll 0)))
   (if (coll? (nth coll 1)) (m-binary-tree? (nth coll 1)) (nil? (nth coll 1)))
   (if (coll? (nth coll 2)) (m-binary-tree? (nth coll 2)) (nil? (nth coll 2)))))

(= (m-binary-tree? '(:a (:b nil nil) nil))
   true)

(= (m-binary-tree? '(:a (:b nil nil)))
   false)

(= (m-binary-tree? [1 nil [2 [3 nil nil] [4 nil nil]]])
   true)

(= (m-binary-tree? [1 [2 nil nil] [3 nil nil] [4 nil nil]])
   false)

(= (m-binary-tree? [1 [2 [3 [4 nil nil] nil] nil] nil])
   true)

(= (m-binary-tree? [1 [2 [3 [4 false nil] nil] nil] nil])
   false)

(= (m-binary-tree? '(:a nil ()))
   false)
