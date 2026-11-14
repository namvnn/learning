(defn m-nil-key? [k m] 
  (and (contains? m k) (nil? (k m))))

(true?  (m-nil-key? :a {:a nil :b 2}))

(false? (m-nil-key? :b {:a nil :b 2}))

(false? (m-nil-key? :c {:a nil :b 2}))
