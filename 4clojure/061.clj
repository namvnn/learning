(defn m-zipmap [keys vals] 
  (reduce 
    (fn [m v] (assoc m (nth v 0) (nth v 1)))
    {}
    (map vector keys vals)))

(= (m-zipmap [:a :b :c] [1 2 3]) {:a 1, :b 2, :c 3})

(= (m-zipmap [1 2 3 4] ["one" "two" "three"]) {1 "one", 2 "two", 3 "three"})

(= (m-zipmap [:foo :bar] ["foo" "bar" "baz"]) {:foo "foo", :bar "bar"})
