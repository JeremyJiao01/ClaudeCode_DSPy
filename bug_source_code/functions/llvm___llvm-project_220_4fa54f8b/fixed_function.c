    if (DenormMode == DenormalMode::getIEEE())
      return nullptr;

    bool IsPositive =
        (!Src.isNegative() || DenormMode.Input == DenormalMode::PositiveZero ||
         (DenormMode.Output == DenormalMode::PositiveZero &&
          DenormMode.Input == DenormalMode::IEEE));
    return ConstantFP::get(CI->getContext(),
                           APFloat::getZero(Src.getSemantics(), !IsPositive));