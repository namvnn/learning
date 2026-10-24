(defn m-identity-keys-and-values [coll]
  (apply hash-map (reduce
                   (fn [acc value]
                     (if (keyword? value)
                       (conj acc value [])
                       (conj (pop acc) (conj (last acc) value))))
                   []
                   coll)))

(= {} (m-identity-keys-and-values []))

(= {:a [1]} (m-identity-keys-and-values [:a 1]))

(= {:a [1]
    :b [2]} (m-identity-keys-and-values [:a 1, :b 2]))

(= {:a [1 2 3]
    :b []
    :c [4]} (m-identity-keys-and-values [:a 1 2 3 :b :c 4]))
