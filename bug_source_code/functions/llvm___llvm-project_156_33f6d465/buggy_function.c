        // references in IR module (not in combined index), so we can
        // ignore them when computing import. We do not export references
        // of writeonly object. See computeImportForReferencedGlobals
        if (ImportIndex.isWriteOnly(GVS) && GVS->refs().size())
          V->setInitializer(Constant::getNullValue(V->getValueType()));
      }
    }