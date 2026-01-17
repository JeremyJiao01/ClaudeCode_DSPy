            if (!diff.minvalue.empty()) {
                ValueFlow::Value value(diff.minvalue.front() - 1);
                value.setImpossible();
                value.bound = ValueFlow::Value::Bound::Upper;
                addToErrorPath(value, diff.minRef);
                result.push_back(value);
            }
            if (!diff.maxvalue.empty()) {
                ValueFlow::Value value(diff.maxvalue.front() + 1);
                value.setImpossible();
                value.bound = ValueFlow::Value::Bound::Lower;
                addToErrorPath(value, diff.maxRef);
                result.push_back(value);
            }