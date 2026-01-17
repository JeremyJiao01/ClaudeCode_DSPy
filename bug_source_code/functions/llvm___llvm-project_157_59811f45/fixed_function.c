        DbgLabels.addInstr(L, MI);
      }

      // Meta Instructions have no output and do not change any values and so
      // can be safely ignored.
      if (MI.isMetaInstruction())
        continue;