(defn m-tricky-card-games [suit]
  (fn [cards]
    (if (nil? suit)
      (let [order {:spade 4 :club 3 :heart 2 :diamond 1}
            f (m-tricky-card-games (:suit (->> cards
                                               (sort-by #((:suit %) order))
                                               (last))))]
        (f cards))
      (->> cards
           (filter #(= (:suit %) suit))
           (sort-by :rank)
           (last)))))

(let [notrump (m-tricky-card-games nil)]
  (notrump [{:suit :spade :rank 2}
            {:suit :club :rank 10}]))

(let [notrump (m-tricky-card-games nil)]
  (and (= {:suit :club :rank 9}  (notrump [{:suit :club :rank 4}
                                           {:suit :club :rank 9}]))
       (= {:suit :spade :rank 2} (notrump [{:suit :spade :rank 2}
                                           {:suit :club :rank 10}]))))

(= {:suit :club :rank 10} ((m-tricky-card-games :club) [{:suit :spade :rank 2}
                                                        {:suit :club :rank 10}]))

(= {:suit :heart :rank 8}
   ((m-tricky-card-games :heart) [{:suit :heart :rank 6} {:suit :heart :rank 8}
                                  {:suit :diamond :rank 10} {:suit :heart :rank 4}]))
