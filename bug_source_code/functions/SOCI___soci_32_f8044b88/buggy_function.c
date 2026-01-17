
    void convert_to_base() SOCI_OVERRIDE
    {
        type_conversion<T>::to_base(value_,
            base_value_holder<T>::val_, ind_);
    }