        return true;
      assert(ID.Kind == ValID::t_MDNode);
      if (ID.MDNodeVal->isFunctionLocal())
        return TokError("unexpected function-local metadata");
      Inst->setMetadata(MDK, ID.MDNodeVal);
    } else {
      unsigned NodeID = 0;