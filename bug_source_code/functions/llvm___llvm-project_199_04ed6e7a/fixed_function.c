                               STI);
  OutStreamer->emitInstruction(
      MCInstBuilder(X86::JMP_1)
          .addExpr(MCSymbolRefExpr::create(ReportError, MCSymbolRefExpr::VK_PLT,
                                           OutContext)),
      STI);
}