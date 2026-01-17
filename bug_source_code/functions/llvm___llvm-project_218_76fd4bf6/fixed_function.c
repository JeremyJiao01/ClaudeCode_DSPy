
    // zext (X == 0) to i32 --> X^1      iff X has only the low bit set.
    // zext (X == 0) to i32 --> (X>>1)^1 iff X has only the 2nd bit set.
    // zext (X != 0) to i32 --> X        iff X has only the low bit set.
    // zext (X != 0) to i32 --> X>>1     iff X has only the 2nd bit set.
    if (Op1CV->isZero() && Cmp->isEquality()) {
      // If Op1C some other power of two, convert:
      KnownBits Known = computeKnownBits(Cmp->getOperand(0), 0, &Zext);

      APInt KnownZeroMask(~Known.Zero);
      if (KnownZeroMask.isPowerOf2()) { // Exactly 1 possible 1?
        bool isNE = Cmp->getPredicate() == ICmpInst::ICMP_NE;
        uint32_t ShAmt = KnownZeroMask.logBase2();
        Value *In = Cmp->getOperand(0);
        if (ShAmt) {
          // Perform a logical shr by shiftamt.
          // Insert the shift to put the result in the low bit.
          In = Builder.CreateLShr(In, ConstantInt::get(In->getType(), ShAmt),
                                  In->getName() + ".lobit");
        }

        if (!isNE) { // Toggle the low bit.
          Constant *One = ConstantInt::get(In->getType(), 1);
          In = Builder.CreateXor(In, One);
        }

        if (Zext.getType() == In->getType())
          return replaceInstUsesWith(Zext, In);

        Value *IntCast = Builder.CreateIntCast(In, Zext.getType(), false);
        return replaceInstUsesWith(Zext, IntCast);