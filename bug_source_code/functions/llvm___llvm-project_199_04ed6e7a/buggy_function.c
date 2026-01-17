                               STI);
  OutStreamer->emitInstruction(
      MCInstBuilder(X86::JMP_1)
          .addExpr(MCSymbolRefExpr::create(ReportError, OutContext)),
      STI);
}
