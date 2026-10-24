(defn m-binary-tree-symmetry? [coll]
  (defn symmetry? [l r]
    (if (not (coll? l)) (= l r)
      (if (not= (first l) (first r)) false
        (let [ll (nth l 1) lr (nth l 2)
              rl (nth r 1) rr (nth r 2)]
          (and
           (symmetry? ll rr)
           (symmetry? lr rl))))))
  (symmetry? (nth coll 1) (nth coll 2)))

(= (m-binary-tree-symmetry? '(:a (:b nil nil) (:b nil nil))) true)

(= (m-binary-tree-symmetry? '(:a (:b nil nil) nil)) false)

(= (m-binary-tree-symmetry? '(:a (:b nil nil) (:c nil nil))) false)

(= (m-binary-tree-symmetry? [1 [2 nil [3 [4 [5 nil nil] [6 nil nil]] nil]]
                             [2 [3 nil [4 [6 nil nil] [5 nil nil]]] nil]])
   true)

(= (m-binary-tree-symmetry? [1 [2 nil [3 [4 [5 nil nil] [6 nil nil]] nil]]
                             [2 [3 nil [4 [5 nil nil] [6 nil nil]]] nil]])
   false)

(= (m-binary-tree-symmetry? [1 [2 nil [3 [4 [5 nil nil] [6 nil nil]] nil]]
                             [2 [3 nil [4 [6 nil nil] nil]] nil]])
   false)
