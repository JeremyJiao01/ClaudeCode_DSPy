        DbgLabels.addInstr(L, MI);
      }

      if (MI.isDebugInstr())
        continue;

      // Not a DBG_VALUE instruction. It may clobber registers which describe