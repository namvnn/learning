(defn m-is-square? [n]
  (let [x (Math/sqrt n)]
    (= (Math/ceil x) (Math/floor x))))
(defn m-filter-square [s]
  (->> (clojure.string/split s #",")
       (map Integer/parseInt)
       (filter #(m-is-square? %1))
       (clojure.string/join ",")))

(= (m-filter-square "4,5,6,7,8,9") "4,9")

(= (m-filter-square "15,16,25,36,37") "16,25,36")
