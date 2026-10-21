(defn m-a-half-truth [& args]
  (and
   (true? (some true? args))
   (true? (some false? args))))

(= false (m-a-half-truth false false))

(= true (m-a-half-truth true false))

(= false (m-a-half-truth true))

(= true (m-a-half-truth false true false))

(= false (m-a-half-truth true true true))

(= true (m-a-half-truth true true true false))
