(defn m-find-anagram [coll]
  (->> (reduce
        (fn [acc item]
          (let [key (clojure.string/join "" (sort item))]
            (if (contains? acc key)
              (update-in acc [key] #(conj % item))
              (assoc acc key #{item}))))
        {} coll)
       (vals)
       (filter #(> (count %) 1))
       (set)))

(= (m-find-anagram ["meat" "mat" "team" "mate" "eat"])
   #{#{"meat" "team" "mate"}})

(= (m-find-anagram ["veer" "lake" "item" "kale" "mite" "ever"])
   #{#{"veer" "ever"} #{"lake" "kale"} #{"mite" "item"}})
