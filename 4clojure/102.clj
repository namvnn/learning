(defn m-into-camel-case [s]
  (if (clojure.string/includes? s "-")
    (let [ss (clojure.string/split s #"-")]
      (clojure.string/join ""
                           (concat []
                                   [(first ss)]
                                   (map #(clojure.string/capitalize %) (rest ss)))))
    s))

(= (m-into-camel-case "something") "something")

(= (m-into-camel-case "multi-word-key") "multiWordKey")

(= (m-into-camel-case "leaveMeAlone") "leaveMeAlone")
