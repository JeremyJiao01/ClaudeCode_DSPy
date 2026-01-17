
    void convert_to_base() SOCI_OVERRIDE
    {
        if (&ind_ != &(base_value_holder<T>::ownInd_) && ind_ == i_null) {
            // An indicator was explicitly passed to use(val, ind), and it was i_null.
            // Therefore, NULL should be put into the DB, and there is no reason to