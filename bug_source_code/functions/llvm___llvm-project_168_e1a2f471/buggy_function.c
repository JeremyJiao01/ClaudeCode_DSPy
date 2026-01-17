      if (!II || Elem.Index == AssumptionCache::ExprResultIdx)
        continue;
      if (RetainedKnowledge RK = getKnowledgeFromBundle(
              *II, II->bundle_op_info_begin()[Elem.Index]))
        if (is_contained(AttrKinds, RK.AttrKind) &&
            Filter(RK, II, &II->bundle_op_info_begin()[Elem.Index])) {
          NumUsefullAssumeQueries++;
          return RK;
        }
    }
    return RetainedKnowledge::none();
  }